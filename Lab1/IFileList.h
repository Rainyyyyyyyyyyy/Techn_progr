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
    virtual void refreshList();
};


