#include "FileMonitor.h"
#include "FileMonitorExceptions.h"

// конструктор по пути к файлу-списку
// в этом моменте старая и новая информация по файлах эквиваентны
// (ввиду того, что создаются в одно и то же время - в момент появления в писке для наблюдения)
FileMonitor::FileMonitor(QString & path_to_hostFile, ILogger * __logg){
    if(checkDotAndDotDot_path(path_to_hostFile)){
        throw ExceptionDotOrDotDotInHostPath();
    }
    QFile File_with_List(path_to_hostFile);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(__logg != nullptr)consoleOutput = __logg;
        else {
            File_with_List.close();
            return;
            }
        pathToHostFile = path_to_hostFile;
        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            add_path(temp_path);
        }
    }else{
        throw new ExceptionUnableToOpenFile; // EXCEPTION_UNABLE_TO_OPEN_FILE;
    }
}

// деструктор
FileMonitor::~FileMonitor(){
    if(consoleOutput != NULL && consoleOutput != nullptr)delete consoleOutput;
    consoleOutput = NULL;
}

// добавить путь к файлу в fileProperties
// returns 1 - added successfully
// returns 0 - path is already exists in fileProperties and has not been added
bool FileMonitor::add_path(QString &path){
    /*QString checkerPath = path;
    QFileInfo chekerPath_fileInfo(checkerPath);
    checkerPath.chop(chekerPath_fileInfo.fileName().size());
    if(checkerPath.contains("..") || checkerPath.contains(".")){
        throw ExceptionDotOrDotDotInListToCheck();
    }
*/
    if(checkDotAndDotDot_path(path)){
        throw ExceptionDotOrDotDotInListToCheck();
    }
    if(checkFileIsHidden_path(path)){
        throw ExceptionFileIsHidden();
    }
    // проверка на существование уже такого пути, во избежание перезаписи
    if(!(filesProperties.contains(path))){
        QFileInfo rawData(path);
        QFileInfo rawData2(path);
        filesProperties[path] = {rawData, rawData2};
        return true;
    }
    else return false;
}

// удалить путь к файлу из fileProperties
// returns 1 - removed successfully
// returns 0 - path is not exists in fileProperties
bool FileMonitor::remove_path(QString &path){
    return filesProperties.remove(path);
}

/*  =   =   =   =   =   ПРОВЕРКИ    =   =   =   =   =   = */

// проверить на наличие '.' и '..'
bool FileMonitor::checkDotAndDotDot_path(QString path) const{
    QFileInfo chekerPath_fileInfo(path);
    path.chop(chekerPath_fileInfo.fileName().size());
    return (path.contains("..") || path.contains("."));
}


// проверить на предмет: файл скрыт
bool FileMonitor::checkFileIsHidden_path(QString &path) const {
    if(checkDotAndDotDot_path(path)){
        throw ExceptionDotOrDotDotInListToCheck();
    }
    QFileInfo ChekerInfo(path);
    ChekerInfo.refresh();
    return ChekerInfo.isHidden();


}
/*  =   =   =   =   =   =   =   =   =   ==  =   =   =   =   =   = */

// перепрочитать (актуализировать) с список путей к файлам с файла-списка
void FileMonitor::refreshList(){
    QFile File_with_List(pathToHostFile);
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

        for(QString &local_path_prev : currentList){
            if(!listOfPaths.contains(local_path_prev)){
                remove_path(local_path_prev);
            }
        }

        for(QString &local_path_new : listOfPaths){
            if(!filesProperties.contains(local_path_new)){
                add_path(local_path_new);
            }
        }
    }else{
        throw new ExceptionUnableToOpenFile;
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
void FileMonitor::Init(QString &path_to_hostFile, ILogger *Logg){
    if(checkDotAndDotDot_path(path_to_hostFile)){
        throw ExceptionDotOrDotDotInHostPath();
    }

    QFile File_with_List(path_to_hostFile);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* проверка указателя *Logg */
        if(Logg != nullptr)consoleOutput = Logg;
        else {
            File_with_List.close();
            return;
        }
        /* Проверка пути к хост-файлу */
        if(pathToHostFile != path_to_hostFile)pathToHostFile = path_to_hostFile;
        else{
            File_with_List.close();
            return;
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
}


void FileMonitor::CheckStateOfFiles(){
    qDebug()<<"===============================================";
        for(auto  & temp_fileinfo : filesProperties){
        // актуализация новых данных fileinfo current_state
            temp_fileinfo.current_state.refresh();
            /* проверка на свойство "файл скрыт" */
            if(temp_fileinfo.current_state.isHidden()){
                throw ExceptionFileIsHidden();
            }
            if(!temp_fileinfo.current_state.exists()){
                // Файл не найден
                emit OnFileLost(temp_fileinfo.current_state.filePath());
            }else{
                if((temp_fileinfo.current_state.size()) != (temp_fileinfo.previous_state.size())){
                    // Размер файла изменился
                    emit OnFileChange(temp_fileinfo.current_state.filePath(), temp_fileinfo.previous_state.size(), temp_fileinfo.current_state.size());
                }else{
                     // Файл существует
                    emit OnFileExists(temp_fileinfo.current_state.filePath(), temp_fileinfo.current_state.size());
                }
            }
            // Обновление старых данных под новые
            temp_fileinfo.previous_state = temp_fileinfo.current_state;
        }
        // обновление списка наблюдаемых файлов
        refreshList();
}


// файл существует
void FileMonitor::OutputEventFileExists(const QString &path, const int &currentSize) const{
    consoleOutput->Log(path + " --- File is exists. Size: " + QString::number(currentSize) + " bytes.");
}

// файл удалён, перемещён или переименован
void FileMonitor::OutputEventFileLost(const QString &path) const {
    consoleOutput->Log(path + " --- File has been deleted, replaced or renamed.");
}

// размер файла изменился на newSize
void FileMonitor::OutputEventFileChanged(const QString &path, const int &oldSize, const int &newSize) const {
    consoleOutput->Log(path + " --- Size has been changed. Size:  " + QString::number(oldSize) + " -> " + QString::number(newSize) + " bytes.");
}



