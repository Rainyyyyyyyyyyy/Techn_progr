#ifndef FILELIST_H
#define FILELIST_H

#endif // FILELIST_H

#include "IFileList.h"
#include <QFile>

class FileList: public IFileList{
private:
    // Список путей к файлам
    QVector <QString> List;

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



     ~FileList();
};

