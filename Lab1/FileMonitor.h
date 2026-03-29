#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#endif // FILEMONITOR_H

#include <QString>
#include <QObject>
#include <QFileInfo>
#include <QVector>
#include <QDebug>
#include <QTextStream>

#include "FileList.h"
#include "Logger.h"

template <class T> void Swap(T &a, T& b){
    T c = a;
    a = b;
    b = c;
}


class FileMonitor : public QObject{

    Q_OBJECT

public:

    // конструктор по умолчанию
    FileMonitor(){}

    // деструктор
    ~FileMonitor(){}

    /*// конструктор по одной строке к одному файлу
    FileMonitor(const QString &QStrPath);

    // конструктор по вектору строк к файлам
    FileMonitor(QVector <QString> &QStrPaths);



    // конструктор по файлу с путями к файлам
    // НЕДОДЕЛАНА
    FileMonitor(const QFile &FileList);



    // файл успешно добавлен под наблюдение - return true
    // else return false
    bool add_file(const QString &path);

    // файл под наблюдение - успешно удаляем из-под наблюдения и return true;
    // else return false
    bool remove_file(const QString &path);
*/

signals:
    // изменение имени или удаление файла равноценно - изменить путь к файлу
    // значит файл изменился, значит файл по прошлому пути можно считать удалённым\утерянным
    void FileLost();

    void SizeChanged(int newSize);


public slots:

    void CheckStateOfFiles();
    /*{
        QTextStream qout(stdout);        // для вывода
        QTextStream qin(stdin);

        QVector <QFileInfo> prev_files = Files;
        unsigned int n = Files.size();
        for(unsigned int i=0; i<n; i++){
            Files[i].refresh();
            if(Files[i].exists() != prev_files[i].exists()
                ||  Files[i].size() != prev_files[i].size()){
                //qDebug().noquote()<<Files[i].path()<<": Changed!\n";
                qout<<Files[i].absolutePath()<<'/'<<Files[i].baseName()<<": Changed!\n"<<Qt::flush;
            }
        }
    }*/


private:
    /*FileList List;*/          // регулировка списка наблюдаемых файлов
    /*Logger ConsoleOutput; */   // вывод
    /* Delayer Delay */ // регулировка задержки




    //QVector <QFileInfo> Files;
    // флаги на изменение (например заменили символ - размер непоменялся, но содержимое изменилось
    //QVector <bool> Files_change_flags;
    // Если создать файл, то QFileInfo сам сделает refresh() (почему-то)
    // поэтому для отслеживания создания файла - флаги на изменение QFileInfo::exists()
    //QVector <bool> Files_arrived_flags;

};
