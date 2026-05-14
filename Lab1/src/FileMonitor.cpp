#include "FileMonitor.h"
#include "FileMonitorExceptions.h"


#include <sys/stat.h>
#include <windows.h>

// анонимное namespace известное этому и только этому файлу (FolderTraveler.cpp)
namespace {
/*
 * Windows:
 *      системный файл ((GetFileAttributes() & FILE_ATTRIBUTE_SYSTEM) == true) -> return true
 *      несистемный файл ((GetFileAttributes() & FILE_ATTRIBUTE_SYSTEM) == false) -> return false
 * Unix:
 *      файл в одной из подсистемных папок:
 *      /proc
 *      /sys
 *      /dev
 *      /run
 *      либо содержит в полном пути эти папки -> return true
 *      файл вне подсистемных папок:
 *       /proc
 *      /sys
 *      /dev
 *      /run
 *      и не содержит в полном пути этих папок -> return false
*/
bool isSystemEntry(const QFileInfo &entry){
#ifdef Q_OS_WIN
    const std::wstring path = entry.absoluteFilePath().toStdWString();
    const DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return (attrs & FILE_ATTRIBUTE_SYSTEM) != 0;
#elif defined(Q_OS_UNIX)
    const QString p = entry.absoluteFilePath();
    if (p == "/proc" || p == "/sys" || p == "/dev" || p == "/run" ||
        p.startsWith("/proc/") || p.startsWith("/sys/") ||
        p.startsWith("/dev/") || p.startsWith("/run/")) {
        return true;
    }

    struct stat st {};
    const QByteArray nativePath = p.toLocal8Bit();
    if (::stat(nativePath.constData(), &st) != 0) {
        return false;
    }

    // Не добавляем специальные узлы ФС (device/fifo/socket).
    return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode) ||
           S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode);
#else
    Q_UNUSED(entry);
    return false;
#endif
}

}




// деструктор
FileMonitor::~FileMonitor(){ }

// добавить путь к файлу в fileProperties
// returns 1 - added successfully
// returns 0 - path is already exists in fileProperties and has not been added
bool FileMonitor::add_path(const QString &path){

    if(path.size() == 0){ return false; }
    if(checkFileIsHidden_path(path)) throw ExceptionFileIsHidden();
    if(checkFileisFile_path(path)) throw ExceptionFilePathIsDirPath();
    if(isSystemEntry(path)) throw ExceptionFileIsFromSystemSpace();

    // проверка на существование уже такого пути, во избежание перезаписи
    if(!(filesProperties.contains(path))){
        QFileInfo rawData(path);
        QFileInfo rawData2(path);
        filesProperties[path] = {rawData, rawData2, 0};
        // не qDebug() а emit в логгер (а то и оттуда же, откуда вызывается функция)
        //qDebug()<<"Path added: "<<path;
        emit signalFileAddedUnderMonitoring("Path added under monitoring: " + path);
        return true;
    }
    else return false;
}

// удалить путь к файлу из fileProperties
// returns 1 - removed successfully
// returns 0 - path is not exists in fileProperties
bool FileMonitor::remove_path(const QString &path){
    if( filesProperties.remove(path)){
        // не qDebug() а emit в логгер (а то и оттуда же, откуда вызывается функция)
        //qDebug()<<"File removed from monitoring: "<<path;
        emit signalFileRemovedFromMonitoring("Path removed from monitoring: " + path);
        return true;
    }
    return false;
}

// проверить, что файл, а не папка
bool FileMonitor::checkFileisFile_path(const QString &path) const{
    QFileInfo CheckerInfo(path);
    CheckerInfo.refresh();
    return CheckerInfo.isDir();
}
// проверить на предмет: файл скрыт
bool FileMonitor::checkFileIsHidden_path(const QString &path) const {
    QFileInfo CheckerInfo(path);
    CheckerInfo.refresh();
    return CheckerInfo.isHidden();
}
/*  =   =   =   =   =   =   =   =   =   ==  =   =   =   =   =   = */

// перепрочитать (актуализировать) с список путей к файлам
void FileMonitor::refreshList(const QVector <QString> &paths){
    /*QFile File_with_List(pathToHostFile);
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
    */
        QList<QString> currentList = filesProperties.keys();

        for(const QString &path_from_prev_list : currentList){
            if(!paths.contains(path_from_prev_list)){
                remove_path(path_from_prev_list);
            }
        }

        for(const QString &local_path_new : paths){
            if(!filesProperties.contains(local_path_new)){
                add_path(local_path_new);
            }
        }
    //}else{
    //    throw ExceptionUnableToOpenFile();
    //}
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
bool FileMonitor::Init(const QVector <QString> &paths, ILogger *Logg){
    /*try{
        if(!path_to_hostFile.endsWith(".txt")){
            throw ExceptionIncorrectFormatOfHostfile();
        }
        if(checkFileIsHidden_path(path_to_hostFile)){
            throw ExceptionHostFileIsHidden();
        }
    /* проверка указателя *Logg
        if(Logg == nullptr){
            return false;
        }
    }catch (CustomExceptions &excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        return false;
    }
    */
        // начинаем читать файл по пути path_to_hostFile
    /*QFile File_with_List(path_to_hostFile);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        pathToHostFile = path_to_hostFile;
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
*/
    // если корректно инициализировались, то отключаем старые сигналы, подключаем новые
    // (во избежание дублирования)
    QObject::disconnect(this, nullptr, nullptr, nullptr);


    QObject::connect(this, &FileMonitor::signalFileAddedUnderMonitoring,    Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileRemovedFromMonitoring,    Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalInitiated,       Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileChange,       Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileExistence,     Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileLost,            Logg, &ILogger::Log);

    /// @brief emit о запуске инициализации
    emit signalInitiated();

    filesProperties.clear();
    for(const QString &temppath : paths){
        add_path(temppath);
    }

}



void FileMonitor::CheckStateOfFiles(){
        for(auto  & temp_fileinfo : filesProperties){
        // актуализация новых данных fileinfo current_state
            temp_fileinfo.current_state.refresh();

            QString tempfilepath = temp_fileinfo.current_state.filePath();
            /* проверка на свойство "файл скрыт" */
            if(checkFileIsHidden_path(tempfilepath))throw ExceptionFileIsHidden();

            /* проверка на свойство "файл имеем системные атрибуты" */
            if(isSystemEntry(tempfilepath)) throw ExceptionFileIsFromSystemSpace();

            // обновление флагов состояния-существования
            temp_fileinfo.exists_flags = ((temp_fileinfo.exists_flags<<1) |  (temp_fileinfo.current_state.exists()))&3;

            // разбор случаев флагов состояния-существования
            switch (temp_fileinfo.exists_flags) {
            case 1:  emit signalFileExistence(temp_fileinfo.current_state.filePath() + " --- exists now."); break;
            case 2:  emit signalFileLost(temp_fileinfo.current_state.filePath() + " --- deleted."); break;
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
        //refreshList(); // вынести в main()
}
