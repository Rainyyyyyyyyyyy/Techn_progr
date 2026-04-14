#include "FileMonitor.h"
//#include "FileList.h"



// конструктор
FileMonitor::FileMonitor(IFileList *__List, ILogger *__Logger, IDelayer *__Delayer){
    if(__List == NULL || __List == nullptr){
        throw new ExceptionIFileListIsNull;
    }
    if(__Logger == NULL || __Logger == nullptr){
        throw new ExceptionILoggerIsNull;
    }
    if(__Delayer == NULL || __Delayer == nullptr){
        throw new ExceptionIDelayerIsNull;
    }
    List = __List;
    ConsoleOutput = __Logger;
    Delay = __Delayer;
}



void FileMonitor::CheckStateOfFiles(){
    unsigned int N = 0;  // = List->getSize();
    QVector <QString> DataPaths; // = List->getList();
    QVector<QFileInfo> oldData;
    QVector<QFileInfo> newData;
    /*for(unsigned int i=0; i<N; i++){
        QFileInfo temp(DataPaths[i]);
        oldData.push_back(temp);
        newData.push_back(temp);
    }*/

    while(true){
        Delay->wait();
        qDebug()<<"===============================================";
        for(unsigned int i=0; i<N; i++){
            newData[i].refresh();
            if(!newData[i].exists()){
                // Файл не найден
                emit OnFileLost(newData[i].filePath());
            }else{
                if((newData[i].size()) != (oldData[i].size())){
                    // Размер файла изменился
                    emit OnFileChange(newData[i].filePath(), oldData[i].size(), newData[i].size());
                }else{
                     // Файл существует
                    emit OnFileExists(newData[i].filePath(), newData[i].size());
                }
            }

            oldData[i] = newData[i];        // Обновление старых данных под новые
            //newData[i].refresh();           // Обновление новых данных на след. итерацию
        }
        List->refreshList();
        N = List->getSize();
        if(N==0)throw new ExceptionFileListIsEmpty;
        //N = List->getSize();
        DataPaths = List->getList();
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
                try{
                    if(temp.absolutePath() == ""){
                        throw new ExceptionFilePathInFileListIsEmpty;
                    }
                }catch(Exceptions *excp){
                    qDebug()<<(excp->what())<<"  Code: "<<excp->getCode();
                    continue;
                }
                newData.push_back(temp);
            }else newData.push_back(oldData[j]);
            //oldData.push_back(temp);
            //newData.push_back(temp);
        }
        oldData.clear();
        oldData = newData;
        qDebug()<<"===============================================";


    }

}



// файл существует
void FileMonitor::OutputEventFileExists(const QString &path, const int &currentSize) const{
    ConsoleOutput->Log(path + " --- File is exists. Size: " + QString::number(currentSize) + " bytes.");
}

// файл удалён, перемещён или переименован
void FileMonitor::OutputEventFileLost(const QString &path) const {
    ConsoleOutput->Log(path + " --- File has been deleted, replaced or renamed");
}

// размер файла изменился на newSize
void FileMonitor::OutputEventFileChanged(const QString &path, const int &oldSize, const int &newSize) const {
    ConsoleOutput->Log(path + " --- Size has been changed. Size:  " + QString::number(oldSize) + " -> " + QString::number(newSize) + " bytes.");
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

