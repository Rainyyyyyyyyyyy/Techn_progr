#include "FileList.h"

#define EXCEPTION_UNABLE_TO_OPEN_FILE 101;


// констрктор по пути к файлу со списком файлов для наблюдения
FileList::FileList(const QString &path_to_File_with_List){
    QFile File_with_List(path_to_File_with_List);

    // open file for reading
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //QFileInfo temp_file;
        //path_to_File_with_List.clear();
        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            //temp_file.setFile(temp_path);
            List.push_back(temp_path);
        }
    }else{
        throw EXCEPTION_UNABLE_TO_OPEN_FILE
    }
}

// конструктор по массиву путей
FileList::FileList(const QVector <QString> &paths){
    List = paths;
}

// конструктор копирования
FileList::FileList(const FileList &s){
    List = s.List;
}



// обновить список (List) файлов по файлу с путями
void FileList::refreshList(const QString &new_path_to_File_with_List){
    QFile File_with_List(new_path_to_File_with_List);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        List.clear();
        //QFileInfo temp_file;
        //path_to_File_with_List.clear();
        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            //temp_file.setFile(temp_path);
            List.push_back(temp_path);
        }
    }else{
        throw EXCEPTION_UNABLE_TO_OPEN_FILE
    }
}

// обновить список (List) файлов по массиву путей
void FileList::refreshList(const QVector <QString> &new_paths){
    List = new_paths;
}



// перегрузка =
FileList & FileList::operator=(const FileList &s){
    if(this == &s)return *this;

    List = s.List;
    return *this;
}
