#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#endif // FILEMONITOR_H

#include <QString>
#include <QObject>
#include <QFileInfo>
#include <QVector>
#include <QDebug>
#include <QTextStream>

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

    // конструктор по пути к файлу-списку
    FileMonitor(QString & path_to_filelist, ILogger * __logg);
    // деструктор
    ~FileMonitor(){
        ///if(List != NULL && List != nullptr)delete List;
        if(consoleOutput != NULL && consoleOutput != nullptr)delete consoleOutput;
        ///if(Delay != NULL && Delay != nullptr)delete Delay;

        ///List = NULL;
        consoleOutput = NULL;
        ///Delay = NULL;
    }



    // геттер
    unsigned int getSize() const;

    QList <QString> /* QVectro <QString> */ getList() const;


signals:
    // изменение имени или удаление файла равноценно - изменить путь к файлу
    // значит файл изменился, значит файл по прошлому пути можно считать удалённым\утерянным

    // файл существует (сообщение что он существует и его размер)
    void OnFileExists(QString path, int currentSize);

    // файл удалён, перемещён или переименован
    void OnFileLost(QString path);

    // размер файла изменился на newSize
    void OnFileChange(QString path, int oldSize, int newSize);


public slots:

    void CheckStateOfFiles();


// файл существует
    void OutputEventFileExists(const QString &path, const int &currentSize) const;
// файл удалён, перемещён или переименован
    void OutputEventFileLost(const QString &path) const;
// размер файла изменился на newSize
    void OutputEventFileChanged(const QString &path, const int &oldSize, const int &newSize) const;



private:

    struct fileStates{
        QFileInfo previous_state;     // информация старая
        QFileInfo current_state;      // информация новая
    };
    // hostFile = File with list of paths to files
    QString path_to_hostFile;
    QMap<QString, fileStates> filesProperties;     ///IFileList *List;                    // список наблюдаемых файлов
    ILogger *consoleOutput;    // вывод
    //IDelayer *Delay;                // регулировка задержки



    // добавить путь к файлу в fileProperties
    bool add_path(QString &path);
    // удалить путь к файлу из fileProperties
    bool remove_path(QString &path);

    // перепрочитать (актуализировать) с список путей к файлам с файла-списка
    void refreshList();



    //QVector <QFileInfo> Files;
    // флаги на изменение (например заменили символ - размер непоменялся, но содержимое изменилось
    //QVector <bool> Files_change_flags;
    // Если создать файл, то QFileInfo сам сделает refresh() (почему-то)
    // поэтому для отслеживания создания файла - флаги на изменение QFileInfo::exists()
    //QVector <bool> Files_arrived_flags;

};
