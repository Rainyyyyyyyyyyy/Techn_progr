#include "FileMonitor.h"

// конструктор по пути к файлу-списку
// в этом моменте старая и новая информация по файлах эквиваентны
// (ввиду того, что создаются в одно и то же время - в момент появления в писке для наблюдения)
FileMonitor::FileMonitor(QString & path_to_filelist){
    QFile File_with_List(path_to_filelist);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        consoleOutput = new ConsoleLogger;
        path_to_hostFile = path_to_filelist;
        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            add_path(temp_path);
            //List.push_back(temp_path);
        }
    }else{
        throw new ExceptionUnableToOpenFile; // EXCEPTION_UNABLE_TO_OPEN_FILE;
    }
}



// добавить путь к файлу в fileProperties
// returns 1 - added successfully
// returns 0 - path is already exists in fileProperties and has not been added
bool FileMonitor::add_path(QString &path){
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

// перепрочитать (актуализировать) с список путей к файлам с файла-списка
void FileMonitor::refreshList(){
    QFile File_with_List(path_to_hostFile);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        //consoleOutput = new ConsoleLogger;
        //path_to_hostFile = path_to_filelist;
        QTextStream File_content(&File_with_List);
        QList <QString> listOfPaths;    // получили из хост-файла только что
        /* прочитали строки из файла */
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            if(!listOfPaths.contains(temp_path))
            listOfPaths.append(temp_path);
            //add_path(temp_path);
            //List.push_back(temp_path);
        }
        File_with_List.close();

        QList currentList = filesProperties.keys(); // имеется в списке-путей до обновления
        for (auto local_path_new : listOfPaths){
            bool flag_is_new = false;
            for(auto local_path_prev : currentList){
                if(local_path_prev == local_path_new){
                    break;
                }
            }
            if(flag_is_new == true){

            }
        }
    }else{
        throw new ExceptionUnableToOpenFile; // EXCEPTION_UNABLE_TO_OPEN_FILE;
    }
}


// getter QList <QString> of paths to files
QList <QString> FileMonitor::getList() const {
    return this->filesProperties.keys();
}

// геттер
unsigned int FileMonitor::getSize() const{
    return static_cast<unsigned int>(filesProperties.size());
}

void FileMonitor::CheckStateOfFiles(){
    //QVector <QString> DataPaths; // = List->getList();
    //QVector<QFileInfo> oldData;
    //QVector<QFileInfo> newData;

    //while(true){
        //Delay->wait();

    //unsigned int N = getSize();  // = List->getSize();
    qDebug()<<"===============================================";
        for(auto  & temp_fileinfo : filesProperties){
        // актуализация новых данных fileinfo current_state
        temp_fileinfo.current_state.refresh();
            //newData[i].refresh();
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
            //newData[i].refresh();           // Обновление новых данных на след. итерацию
        }
}
        /*List->refreshList();
        N = /*List->getSize();
        if(N==0)throw new ExceptionFileListIsEmpty;
        //N = List->getSize();
        /////////////////////////////////////////////////////////DataPaths =/* List->getList();
        //oldData.clear();
        newData.clear();
        //QVector<QFileInfo> oldData;
        //QVector<QFileInfo> newData;
        unsigned int old_N = oldData.size();
        for(unsigned int i=0; i<N; i++){
            QFileInfo temp(DataPaths[i]);
            bool flag_not_new = true;
            unsigned int j = 0;
            for(j=0; j<old_N; j++){
                if(temp.absoluteFilePath() == oldData[j].absoluteFilePath()){
                    flag_not_new = false;
                    break;
                }
            }
            if(flag_not_new == true){
                newData.push_back(temp);
            }else newData.push_back(oldData[j]);
            //oldData.push_back(temp);
            //newData.push_back(temp);
        }
        oldData.clear();
        oldData = newData;
        qDebug()<<"===============================================";


}

*/

// файл существует
void FileMonitor::OutputEventFileExists(const QString &path, const int &currentSize) const{
    consoleOutput->Log(path + " --- File is exists. Size: " + QString::number(currentSize) + " bytes.");
}

// файл удалён, перемещён или переименован
void FileMonitor::OutputEventFileLost(const QString &path) const {
    consoleOutput->Log(path + " --- File has been deleted, replaced or renamed");
}

// размер файла изменился на newSize
void FileMonitor::OutputEventFileChanged(const QString &path, const int &oldSize, const int &newSize) const {
    consoleOutput->Log(path + " --- Size has been changed. Size:  " + QString::number(oldSize) + " -> " + QString::number(newSize) + " bytes.");
}









/*
// конструктор по одной строке к одному файлу
FileMonitor::FileMonitor(const QString &QStrPath){
    this->add_file(QStrPath);
}



// конструктор по вектору строк к файлам
FileMonitor::FileMonitor(QVector <QString> &QStrPaths){
    for(const QString &path : QStrPaths){
        this->add_file(path);
    }
}



// конструктор по файлу с путями к файлам
// НЕДОДЕЛАНА
FileMonitor::FileMonitor(const QFile &FileList){
    if(!FileList.exists()){
        throw "Error with FileList: incorrect FileList's path"; // catch в main()
        return;
    }
    // скрипт считывания с файла FileList путей в QVector <QFileInfo> Files


}



// файл успешно добавлен под наблюдение - return true
// else return false
bool FileMonitor::add_file(const QString &path){
    bool flag_new = true;
    for(int i=0; i<Files.size(); i++){
        if(Files[i].absolutePath() == path){
            flag_new = false;
            break;
        }
    }
    // добавляемый путь - новый
    if(flag_new == true){
        QFileInfo temp(path);
        Files.push_back(temp);
        Files_arrived_flags.push_back(false);
        Files_change_flags.push_back(false);
    }
    return flag_new;
}



// файл под наблюдение - успешно удаляем из-под наблюдения и return true;
// else return false
bool FileMonitor::remove_file(const QString &path){
    bool flag_is_watching = false;
    int i=0;
    for(i; i<Files.size(); i++){
        if(Files[i].absolutePath() == path){
            flag_is_watching = true;
            break;
        }
    }

    if(flag_is_watching == true){
        for(i; i<Files.size()-1; i++){
            Swap(Files[i], Files[i+1]);
            Swap(Files_arrived_flags[i], Files_arrived_flags[i+1]);
            Swap(Files_change_flags[i], Files_change_flags[i+1]);
        }
        Files.pop_back();
        return true;
    }else return false;

}


//
void FileMonitor::CheckStateOfFiles(){
    //QTextStream qout(stdout);        // для вывода

    QVector <QFileInfo> prev_files = Files;
    unsigned int n = Files.size();
    for(unsigned int i=0; i<n; i++){
        Files[i].refresh();
        if(Files[i].exists() != prev_files[i].exists()
            ||  Files[i].size() != prev_files[i].size()){
            //qDebug().noquote()<<Files[i].path()<<": Changed!\n";
            qDebug()<<Files[i].absolutePath()<<'/'<<Files[i].baseName()<<": Changed!\n"<<Qt::flush;
        }
    }
}

*/

