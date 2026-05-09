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
    explicit FileMonitor (QObject *parent = nullptr) : QObject(parent) { pathToHostFile = ""; }


    // деструктор
    ~FileMonitor();

    // проверить состояние файлов - сравнение старой и новой информации и логирование событий
    void CheckStateOfFiles();

    // Установка пути к хост-файлу и сущности-вывода
    bool Init(QString & path_to_hostFile, ILogger * Logg);

    // геттеры
    unsigned int getSize() const;
    QList <QString> getList() const;


signals:
    // изменение имени или удаление файла равноценно - изменить путь к файлу
    // значит файл изменился, значит файл по прошлому пути можно считать удалённым\утерянным

    // файл появился или удалился (сообщение что он существует и его размер)
    void signalFileExistence(QString msg_log);//(QString path, int currentSize);

    // файл удалён, перемещён или переименован
    void signalFileLost(QString msg_log);//(QString path);

    // размер файла изменился на newSize
    void signalFileChange(QString msg_log);//(QString path, int oldSize, int newSize);


private:

    struct fileStates{
        QFileInfo previous_state;     // информация старая
        QFileInfo current_state;      // информация новая
        unsigned char exists_flags;     // bit_1 - old info. bit_0 - new info: 00 - deleted\deleted as-well     01 - deleted\arrived!   10 - exists\deleted     11 - exists\exists as-well
    };
    // hostFile - Файл, в котором перечислены по-строчно абсолютные пути к файлам для наблюдения
    QString pathToHostFile;
    QMap<QString, fileStates> filesProperties;  // список наблюдаемых файлов



    // проверить на наличие '.' и '..'
    //bool checkDotAndDotDot_path(QString path) const;
    // проверить на предмет: файл скрыт
    bool checkFileIsHidden_path(QString &path) const;
    // проверить, что файл, а не папка
    bool checkFileisFile_path(QString &path) const;
    // добавить путь к файлу в fileProperties
    bool add_path(QString &path);
    // удалить путь к файлу из fileProperties
    bool remove_path(QString &path);
    // перепрочитать (актуализировать) с список путей к файлам с файла-списка
    void refreshList();



};
