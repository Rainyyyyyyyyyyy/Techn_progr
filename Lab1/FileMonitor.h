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
#include "Delayer.h"

#include "FileMonitorExceptions.h"


template <class T> void Swap(T &a, T& b){
    T c = a;
    a = b;
    b = c;
}


class FileMonitor : public QObject{

    Q_OBJECT

public:
    explicit FileMonitor (QObject *parent = nullptr) : QObject(parent) {}
    // конструктор по умолчанию
    //FileMonitor(){}

    // конструктор
    FileMonitor(IFileList *__List = nullptr, ILogger *__Logger = nullptr, IDelayer *__Delayer = nullptr);

    // деструктор
    ~FileMonitor(){
        if(List != NULL && List != nullptr)delete List;
        if(ConsoleOutput != NULL && ConsoleOutput != nullptr)delete ConsoleOutput;
        if(Delay != NULL && Delay != nullptr)delete Delay;

        List = NULL;
        ConsoleOutput = NULL;
        Delay = NULL;
    }

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

    // файл существует (сообщение что он существует и его размер)
    void OnFileExists(QString path, int currentSize);

    // файл удалён, перемещён или переименован
    void OnFileLost(QString path);

    // размер файла изменился на newSize
    void OnFileChange(QString path, int oldSize, int newSize);

    /*
    // debug
    void smth_changed(){
        ConsoleOutput->Log("Debug output");
    }
*/

public slots:

    void CheckStateOfFiles();


// файл существует
    void OutputEventFileExists(const QString &path, const int &currentSize) const;
// файл удалён, перемещён или переименован
    void OutputEventFileLost(const QString &path) const;
// размер файла изменился на newSize
    void OutputEventFileChanged(const QString &path, const int &oldSize, const int &newSize) const;



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
    IFileList *List;                    // список наблюдаемых файлов
    ILogger *ConsoleOutput;    // вывод
    IDelayer *Delay;                // регулировка задержки




    //QVector <QFileInfo> Files;
    // флаги на изменение (например заменили символ - размер непоменялся, но содержимое изменилось
    //QVector <bool> Files_change_flags;
    // Если создать файл, то QFileInfo сам сделает refresh() (почему-то)
    // поэтому для отслеживания создания файла - флаги на изменение QFileInfo::exists()
    //QVector <bool> Files_arrived_flags;

};
