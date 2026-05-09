#include "FileMonitor.h"
#include "FileMonitorExceptions.h"

// деструктор
FileMonitor::~FileMonitor(){ }

// добавить путь к файлу в fileProperties
// returns 1 - added successfully
// returns 0 - path is already exists in fileProperties and has not been added
bool FileMonitor::add_path(QString &path){

    if(path.size() == 0){ return false; }
    if(checkFileIsHidden_path(path)){
        throw ExceptionFileIsHidden();
    }
    if(checkFileisFile_path(path)){
        throw ExceptionFilePathIsDirPath();
    }
    // проверка на существование уже такого пути, во избежание перезаписи
    if(!(filesProperties.contains(path))){
        QFileInfo rawData(path);
        QFileInfo rawData2(path);
        filesProperties[path] = {rawData, rawData2, 0};
        qDebug()<<"Path added: "<<path;
        return true;
    }
    else return false;
}

// удалить путь к файлу из fileProperties
// returns 1 - removed successfully
// returns 0 - path is not exists in fileProperties
bool FileMonitor::remove_path(QString &path){
    if( filesProperties.remove(path)){
        qDebug()<<"File removed from monitoring: "<<path;
        return true;
    }
    return false;
}

// проверить, что файл, а не папка
bool FileMonitor::checkFileisFile_path(QString &path) const{
    QFileInfo CheckerInfo(path);
    CheckerInfo.refresh();
    return CheckerInfo.isDir();
}
// проверить на предмет: файл скрыт
bool FileMonitor::checkFileIsHidden_path(QString &path) const {
    QFileInfo CheckerInfo(path);
    CheckerInfo.refresh();
    return CheckerInfo.isHidden();
}
/*  =   =   =   =   =   =   =   =   =   ==  =   =   =   =   =   = */

// перепрочитать (актуализировать) с список путей к файлам с файла-списка
void FileMonitor::refreshList(){
    QFile File_with_List(pathToHostFile);
    if(checkFileIsHidden_path(pathToHostFile)){
        throw ExceptionFileIsHidden();
    }
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream File_content(&File_with_List);
        QList <QString> listOfPaths;    // пути из хост-файла без повторов

        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            if(!temp_path.isEmpty() && !listOfPaths.contains(temp_path)){
                listOfPaths.append(temp_path);
            }
        }
        File_with_List.close();
        if(listOfPaths.empty()){
            throw ExceptionFileListIsEmpty();
        }
        QList<QString> currentList = filesProperties.keys();

        for(QString &path_from_prev_list : currentList){
            if(!listOfPaths.contains(path_from_prev_list)){
                remove_path(path_from_prev_list);
            }
        }

        for(QString &local_path_new : listOfPaths){
            if(!filesProperties.contains(local_path_new)){
                add_path(local_path_new);
            }
        }
    }else{
        throw ExceptionUnableToOpenFile();
    }
}


// getter QList <QString> of paths to files
QList <QString> FileMonitor::getList() const {
    return this->filesProperties.keys();
}

// геттер size
unsigned int FileMonitor::getSize() const{
    return static_cast<unsigned int>(filesProperties.size());
}

// Установка пути к хост-файлу и сущности-вывода
// если ILogger * ==nullptr или путь к хост-файлу некорректный или не изменился,
// то инициализация не происходит, а исключение не выбрасывается и не отсоединяются старые сигналы
bool FileMonitor::Init(QString &path_to_hostFile, ILogger *Logg){
    if(!path_to_hostFile.endsWith(".txt")){
        throw ExceptionIncorrectFormatOfHostfile();
    }
    if(checkFileIsHidden_path(path_to_hostFile)){
        throw ExceptionHostFileIsHidden();
    }
    /* проверка указателя *Logg */
    if(Logg == nullptr){
        return false;
    }

        // начинаем читать файл по пути path_to_hostFile
    QFile File_with_List(path_to_hostFile);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* Проверка пути к хост-файлу */
        if(pathToHostFile != path_to_hostFile)pathToHostFile = path_to_hostFile;
        else{
            File_with_List.close();
            return false;
        }
        // очистка информации о прошлом списке
        filesProperties.clear();

        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            add_path(temp_path);
        }
    }else{
        throw ExceptionUnableToOpenFile();
    }
    // если корректно инициализировались, то отключаем старые сигналы, подключаем новые
    // (во избежание дублирования)
    QObject::disconnect(this, nullptr, nullptr, nullptr);
    QObject::connect(this, &FileMonitor::signalFileChange,       Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileExistence,     Logg, &ILogger::Log);

}



void FileMonitor::CheckStateOfFiles(){
        for(auto  & temp_fileinfo : filesProperties){
        // актуализация новых данных fileinfo current_state
            temp_fileinfo.current_state.refresh();
            /* проверка на свойство "файл скрыт" */
            QString tempfilepath = temp_fileinfo.current_state.filePath();
            if(checkFileIsHidden_path(tempfilepath)){
                throw ExceptionFileIsHidden();
            }

            // обновление флагов состояния-существования
            temp_fileinfo.exists_flags = ((temp_fileinfo.exists_flags<<1) |  (bool)(temp_fileinfo.current_state.exists()))&3;

            // разбор случаев флагов состояния-существования
            switch (temp_fileinfo.exists_flags) {
            case 1:  emit signalFileExistence(temp_fileinfo.current_state.filePath() + " --- exists now."); break;
            case 2:  emit signalFileExistence(temp_fileinfo.current_state.filePath() + " --- deleted."); break;
            default: break;
            }

            if((temp_fileinfo.current_state.size()) != (temp_fileinfo.previous_state.size()) && temp_fileinfo.exists_flags == 3){
                    // Размер файла изменился ( и файл продолжает существовать )
                emit signalFileChange(temp_fileinfo.current_state.filePath() +
                                                                " --- Size has been changed. Size:  " +
                                                                QString::number(temp_fileinfo.previous_state.size()) +
                                                                " -> " + QString::number(temp_fileinfo.current_state.size()) + " bytes.");
                }

            // Обновление старых данных под новые
            temp_fileinfo.previous_state = temp_fileinfo.current_state;
        }
        // обновление списка наблюдаемых файлов
        refreshList();
}
