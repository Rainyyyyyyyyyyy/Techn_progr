#include "FileMonitor.h"
#include "FileMonitorExceptions.h"


#include <sys/stat.h>
#include <windows.h>



// деструктор
FileMonitor::~FileMonitor(){ }

// добавить путь к файлу в fileProperties
// returns 1 - added successfully
// returns 0 - path is already exists in fileProperties and has not been added
bool FileMonitor::addPath(const QString &path){

    if(path.size() == 0){ return false; }
    if(checkHiddenPath(path)) throw ExceptionFileIsHidden();
    if(checkFilePath(path)) throw ExceptionFilePathIsDirPath();
    if(checkSystemPath(path))throw ExceptionFileIsFromSystemSpace();


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
bool FileMonitor::removePath(const QString &path){
    if(filesProperties.remove(path)){
        emit signalFileRemovedFromMonitoring("Path removed from monitoring: " + path);
        return true;
    }
    return false;
}

// проверить, что файл, а не папка
bool FileMonitor::checkFilePath(const QString &path) const{
    QFileInfo CheckerInfo(path);
    CheckerInfo.refresh();
    return CheckerInfo.isDir();
}

// проверить на предмет: файл скрыт
bool FileMonitor::checkHiddenPath(const QString &path) const {
    QFileInfo CheckerInfo(path);
    CheckerInfo.refresh();
    return CheckerInfo.isHidden();
}

//
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
bool FileMonitor::checkSystemPath(const QString &path) const{
#ifdef Q_OS_WIN
    const std::wstring p = path.toStdWString();
    //const std::wstring path = entry.absoluteFilePath().toStdWString();
    const DWORD attrs = GetFileAttributesW(p.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return (attrs & FILE_ATTRIBUTE_SYSTEM) != 0;
#elif defined(Q_OS_UNIX)
    //const QString p = entry.absoluteFilePath();
    if (path == "/proc" || path == "/sys" || path == "/dev" || path == "/run" ||
        path.startsWith("/proc/") || path.startsWith("/sys/") ||
        path.startsWith("/dev/") || path.startsWith("/run/")) {
        return true;
    }

    struct stat st {};
    const QByteArray nativePath = path.toLocal8Bit();
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
/*  =   =   =   =   =   =   =   =   =   ==  =   =   =   =   =   = */

// перепрочитать (актуализировать) список путей к файлам (по поступившему новому QVector <QString> paths)
void FileMonitor::refreshList(const QVector <QString> &paths){
        QList<QString> currentList = filesProperties.keys();

        for(const QString &path_from_prev_list : currentList){
            if(!paths.contains(path_from_prev_list)){
                removePath(path_from_prev_list);
            }
        }

        for(const QString &local_path_new : paths){
            if(!filesProperties.contains(local_path_new)){
                addPath(local_path_new);
            }
        }
}


// getter QList <QString> путей к файлам
QList <QString> FileMonitor::getList() const {
    return this->filesProperties.keys();
}

// getter размера списка путей
unsigned int FileMonitor::getSize() const{
    return static_cast<unsigned int>(filesProperties.size());
}

// Установка пути к хост-файлу и сущности-вывода
// если ILogger * ==nullptr или путь к хост-файлу некорректный или не изменился,
// то инициализация не происходит, а исключение не выбрасывается и не отсоединяются старые сигналы
bool FileMonitor::Init(const QVector <QString> &paths, ILogger *Logg){
    // если корректно инициализировались, то отключаем старые сигналы, подключаем новые
    // (во избежание дублирования)
    if(Logg == nullptr || Logg == NULL){
            return false;
    }

    QObject::disconnect(this, nullptr, nullptr, nullptr);


    QObject::connect(this, &FileMonitor::signalFileAddedUnderMonitoring,    Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileRemovedFromMonitoring,    Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalInitiated,       Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileChange,       Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileExists,     Logg, &ILogger::Log);
    QObject::connect(this, &FileMonitor::signalFileLost,            Logg, &ILogger::Log);


    emit signalInitiated();
    filesProperties.clear();
    for(const QString &temppath : paths){
        addPath(temppath);
    }
}



void FileMonitor::CheckStateOfFiles(){
        for(auto  & temp_fileinfo : filesProperties){
        // актуализация новых данных fileinfo current_state
            temp_fileinfo.current_state.refresh();

            QString tempfilepath = temp_fileinfo.current_state.filePath();
            /* проверка на свойство "файл скрыт" */
            if(checkHiddenPath(tempfilepath))throw ExceptionFileIsHidden();

            /* проверка на свойство "файл имеем системные атрибуты" */
            if(checkSystemPath(tempfilepath)) throw ExceptionFileIsFromSystemSpace();

            // обновление флагов состояния-существования
            temp_fileinfo.exists_flags = ((temp_fileinfo.exists_flags<<1) |  (temp_fileinfo.current_state.exists()))&3;

            // разбор случаев флагов состояния-существования
            switch (temp_fileinfo.exists_flags) {
            case 1:  emit signalFileExists(temp_fileinfo.current_state.filePath() + " --- exists now."); break;
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
}


