#include "FileList.h"
#include "FileListExceptions.hpp"


// констрктор по пути к файлу со списком файлов для наблюдения
FileList::FileList(const QString &path_to_File_with_List){
    PathToList = path_to_File_with_List;
    QFile File_with_List(path_to_File_with_List);


    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        List.clear();
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

/*
// конструктор по массиву путей
FileList::FileList(const QVector <QString> &paths){
    PathToList = "";
    List.clear();
    for(unsigned int i=0; i<paths.size(); i++){
        add_path(paths[i]);
    }
    //List = paths;
}
*/
// конструктор копирования
FileList::FileList(const FileList &s){
    List = s.List;
}


/*
// обновить список (List) файлов по файлу с путями
void FileList::refreshList(const QString &new_path_to_File_with_List){
    PathToList = new_path_to_File_with_List;
    QFile File_with_List(new_path_to_File_with_List);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        List.clear();
        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            add_path(temp_path);
            //List.push_back(temp_path);
        }
    }else{
        throw new ExceptionUnableToOpenFile;    // EXCEPTION_UNABLE_TO_OPEN_FILE;
    }
    File_with_List.close();
}

// обновить список (List) файлов по массиву путей
void FileList::refreshList(const QVector <QString> &new_paths){
    PathToList = "";
    List.clear();
    for(unsigned int i=0; i<new_paths.size(); i++){
        add_path(new_paths[i]);
    }
}
*/
void FileList::refreshList(){
    if(PathToList == ""){
        throw new ExceptionFilePathToListIsEmpty;
    }
    QFile File_with_List(PathToList);
    if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        List.clear();
        QTextStream File_content(&File_with_List);
        while(!File_content.atEnd()){
            QString temp_path = File_content.readLine();
            if(!temp_path.isEmpty())add_path(temp_path);
            //List.push_back(temp_path);
        }
    }else{
        throw new ExceptionUnableToOpenFile;    // EXCEPTION_UNABLE_TO_OPEN_FILE;
    }
    File_with_List.close();
}


// перегрузка =
FileList & FileList::operator=(const FileList &s){
    if(this == &s)return *this;

    List = s.List;
    PathToList = s.PathToList;
    return *this;
}



// getter списка
const QVector <QString> & FileList::getList() const {
    return List;
}

const QString & FileList::getPathToList() const {
    return PathToList;
}

// size
unsigned int FileList::getSize() const{
    return List.size();
}


// оператор[]
QString & FileList::operator[](const unsigned int &index){
    if(index >= static_cast<unsigned int>(List.size())){
        throw new ExceptionIndexOutOfBounds; //EXCEPTION_INDEX_OUT_OF_BOUNDS;
    }
    return List[index];
}

// добавить в список путь к файлу (return true)
// если файл уже есть, то не добавлять (return false)
bool FileList::add_path(const QString &path){
    unsigned int N = static_cast<unsigned int>(List.size());
    for(unsigned int i=0; i<N; i++){
        if(path == List[i]){
            return false;
        }
    }
    List.push_back(path);
    return true;
}



/* Тестирование
 *  // констрктор по пути к файлу со списком файлов для наблюдения
FileList::FileList(const QString &path_to_File_with_List)

На вход: путь к несуществующему файлу - выход: throw EXCEPTION_UNABLE_TO_OPEN_FILE
На вход: путь к существующему непустому файлу  - выход: заполненный QVector <QString> List строками из файла без повторений (из-за add_path())
На вход: путь к существующему пустому файлу - выход: пустой QVector <QString> List

// конструктор по массиву путей
FileList::FileList(const QVector <QString> &paths)
копирование paths ---> *this.List без повторений (из-за add_path())

// конструктор копирования
FileList::FileList(const FileList &s)
копирование (оператор= для QVector) s.List ---> *this.List

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

// добавить в список путь к файлу (return true)
// если файл уже есть, то не добавлять (return false)
bool FileList::add_path(const QString &path)
На вход: пустая строка при отсутствии таковой в List - выход: return true; добавление в List пустой строки
На вход: пустая строка при наличии таковой в List - выход: return false; List не изменился
На вход: любая строка при пустом List - выход: return true; добавление в List полученной строки
На вход: любая строка, которая уже есть в List - выход: return false; List не изменился
*/
