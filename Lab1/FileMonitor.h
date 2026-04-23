#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#endif // FILEMONITOR_H

#include <QString>
#include <QObject>
#include <QFileInfo>
#include <QVector>
#include <QMap>
#include <QDebug>
#include <QTextStream>


#include "ConsoleLogger.h"


class FileMonitor : public QObject{

    Q_OBJECT

public:
    explicit FileMonitor (QObject *parent = nullptr) : QObject(parent) {}

    // конструктор по пути к файлу-списку
    FileMonitor(QString & path_to_hostFile, ILogger * __logg);
    // деструктор
    ~FileMonitor();

    // проверить состояние файлов - сравнение старой и новой информации и логирование событий
    void CheckStateOfFiles();

    // Установка пути к хост-файлу и сущности-вывода
    void Init(QString & path_to_hostFile, ILogger * Logg);

    // геттеры
    unsigned int getSize() const;
    QList <QString> getList() const;


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
    // hostFile - Файл, в котором перечислены по-строчно абсолютные пути к файлам для наблюдения
    QString pathToHostFile;
    QMap<QString, fileStates> filesProperties;  // список наблюдаемых файлов
    ILogger *consoleOutput;    // вывод


    // проверить на наличие '.' и '..'
    bool checkDotAndDotDot_path(QString path) const;
    // проверить на предмет: файл скрыт
    bool checkFileIsHidden_path(QString &path) const;
    // добавить путь к файлу в fileProperties
    bool add_path(QString &path);
    // удалить путь к файлу из fileProperties
    bool remove_path(QString &path);
    // перепрочитать (актуализировать) с список путей к файлам с файла-списка
    void refreshList();



};
