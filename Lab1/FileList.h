#ifndef FILELIST_H
#define FILELIST_H

#endif // FILELIST_H

#include "IFileList.h"
#include "FileListExceptions.hpp"
#include <QFile>


class FileList: public IFileList {
private:
    // Список путей к файлам
    QVector <QString> List;



    // добавить в список путь к файлу (return true)
    // если файл уже есть, то не добавлять (return false)
    /// (будет использоваться в FileList::refreshList(...)) и конструкторах
    bool add_path(const QString &path);
public:

    FileList(){}

    // конструктор по файлу с путями
    FileList(const QString &path_to_File_with_List);

    // конструктор по массиву путей
    FileList(const QVector <QString> &paths);

    // конструктор копирования
    FileList(const FileList &s);

    // перегрузка =
    FileList &operator=(const FileList &s);

    void refreshList(const QString &new_path_to_File_with_List);
    void refreshList(const QVector <QString> &new_paths);

    // getter списка
    const QVector <QString> &get_list() const;

    // оператор[]
    QString & operator[](const unsigned int &index);

    // getter
    const QVector <QString> &getList();

    // size
    unsigned int getSize() const;

    ~FileList() {}
};

