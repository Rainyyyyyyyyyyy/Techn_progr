#ifndef IFILELIST_H
#define IFILELIST_H

#endif // IFILELIST_H

#include <QVector>
#include <QString>
#include <qDebug>
#include <QFileInfo>

class IFileList{
public:
    virtual ~IFileList() = default;
    // как минимум конструкторы по пути к файлу со списком
    //                                          и       по массиву-списку
    virtual void refreshList(const QString &); // путь до файла со списком файлов для наблюдения
    virtual void refreshList(const QVector <QString> &); // массив путей до файлов для наблюдения
};


