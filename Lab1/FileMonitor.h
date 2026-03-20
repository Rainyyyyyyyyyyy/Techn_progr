#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#endif // FILEMONITOR_H

#include <QString>
#include <QObject>
#include <QFileInfo>
#include <QVector>
#include <QDebug>

class FileMonitor : public QObject{

    Q_OBJECT

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


public slots:
    void CheckStateOfFiles(){
        QVector <QFileInfo> prev_files = Files;
        unsigned int n = Files.size();
        for(unsigned int i=0; i<n; i++){
            Files[i].refresh();
            if(Files[i].exists() != prev_files[i].exists()
                ||  Files[i].size() != prev_files[i].size()){
                qDebug().noquote()<<Files[i].path()<<": Changed!\n";
            }
        }
    }


private:
    QVector <QFileInfo> Files;

};
