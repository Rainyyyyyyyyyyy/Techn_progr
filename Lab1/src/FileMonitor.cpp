#include "FileMonitor.h"
//#include "FileList.h"

#define EXCEPTION_IFILELIST_IS_NULLPTR 102
#define EXCEPTION_ILOGGER_IS_NULLPTR 103
#define EXCEPTION_IDELAYER_IS_NULLPTR 104

// конструктор
FileMonitor::FileMonitor(IFileList *__List, ILogger *__Logger, IDelayer *__Delayer){
    if(__List == NULL || __List == nullptr){
        throw EXCEPTION_IFILELIST_IS_NULLPTR;
    }
    if(__Logger == NULL || __Logger == nullptr){
        throw EXCEPTION_ILOGGER_IS_NULLPTR;
    }
    if(__Delayer == NULL || __Delayer == nullptr){
        throw EXCEPTION_IDELAYER_IS_NULLPTR;
    }
    List = __List;
    ConsoleOutput = __Logger;
    Delay = __Delayer;
}



void FileMonitor::CheckStateOfFiles(){
    unsigned int N = List->getSize();
    QVector <QString> DataPaths = List->getList();
    QVector<QFileInfo> oldData;
    QVector<QFileInfo>newData;
    for(unsigned int i=0; i<N; i++){
        QFileInfo temp(DataPaths[i]);
        oldData.push_back(temp);
        newData.push_back(temp);
    }
    while(true)
    {
        for(unsigned int i=0; i<N; i++){
            newData[i].refresh();
            if(oldData[i].exists() != newData[i].exists()){
                QString local_msg = (oldData[i].exists())? "Lost" : "Arrived";
                qDebug()<<newData[i].path()<<" : "<<local_msg;
            }
        }
    }
    /// ДОДЕЛАТЬ
    ///
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

