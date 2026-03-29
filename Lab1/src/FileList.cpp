#include "FileList.h"

#define EXCEPTION_UNABLE_TO_OPEN_FILE 101;


// констрктор по пути к файлу со списком файлов для наблюдения
FileList::FileList(const QString &path_to_File_with_List){
    QFile File_with_List(path_to_File_with_List);


    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
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
        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
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



/* Тестирование
 *  // констрктор по пути к файлу со списком файлов для наблюдения
FileList::FileList(const QString &path_to_File_with_List)

На вход: путь к несуществующему файлу - выход: throw EXCEPTION_UNABLE_TO_OPEN_FILE
На вход: путь к существующему непустому файлу  - выход: заполненный QVector <QString> List строками из файла
На вход: путь к существующему пустому файлу - выход: пустой QVector <QString> List

// конструктор по массиву путей
FileList::FileList(const QVector <QString> &paths)
копирование QVector <QString> в QVector <QString> (оператор = для QVector)

// конструктор копирования
FileList::FileList(const FileList &s)
Аналогично конструктору по массиву путей

// обновить список (List) файлов по файлу с путями
void FileList::refreshList(const QString &new_path_to_File_with_List)
Аналогично конструктору по пути к фалу со списком путей для наблюдения

// обновить список (List) файлов по массиву путей
void FileList::refreshList(const QVector <QString> &new_paths)
Аналогично конструктору по массиву путей

// перегрузка =
FileList & FileList::operator=(const FileList &s)
На вход: &s == this - выход: this без изменений, возвращение this
На вход: &s != this - выход: this->List присваивается s.List (оператор= для QVector)
Поведение для List - оператор= для QVector
*/
