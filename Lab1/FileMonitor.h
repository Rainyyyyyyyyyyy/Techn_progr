#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#endif // FILEMONITOR_H

#include <QString>
#include <QObject>
#include <QFileInfo>
#include <QVector>

class FileMonitor : public QObject{

    Q_OBJECT

    QVector <QFileInfo> Files;

public:

    // конструктор по умолчанию
    FileMonitor(){}

    // конструктор по одной строке к одному файлу
    FileMonitor(const QString &QStrPath){
        QFileInfo fileinfo(QStrPath);
        Files.push_back(fileinfo);
    }

    // конструктор по вектору строк к файлам
    FileMonitor(QVector <QString> &QStrPaths){
        for(QString path : QStrPaths){
            Files.push_back(QFileInfo(path));
        }
    }

signals:
    // изменение имени или удаление файла равноценно - изменить путь к файлу
    // значит файл изменился, значит файл по прошлому пути можно считать удалённым\утерянным
    void FileLost();

    void SizeChanged(int newSize);



};
