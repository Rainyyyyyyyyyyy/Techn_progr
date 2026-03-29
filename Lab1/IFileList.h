#ifndef IFILELIST_H
#define IFILELIST_H

#endif // IFILELIST_H

#include <QVector>
#include <QString>
#include <qDebug>
#include <QFileInfo>

// предполагается, что список хранится в массиве\векторе
template <typename T>
class IFileList{
public:
    virtual ~IFileList() = default;
    // как минимум конструкторы по пути к файлу со списком
    //                                          и       по массиву-списку
    virtual void refreshList(const T &); // путь до файла со списком файлов для наблюдения
    virtual void refreshList(const QVector <T> &); // массив путей до файлов для наблюдения

    // getter
    virtual const QVector <T> &getList();

    // size
    virtual const unsigned int getSize() const;
};


