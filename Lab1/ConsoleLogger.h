#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H


#include "ILogger.h"
#include <QDebug>


class ConsoleLogger : public ILogger{
private:



public:

    ConsoleLogger() {};
    ~ConsoleLogger() {};



    // переопределение log от базового класса

    void Log(QString s) override;
public:
    // [слот] файл существует
    void OutputEventFileExists(const QString &path, const int &currentSize);

    // [слот] файл удалён, перемещён или переименован
    void OutputEventFileLost(const QString &path);

    // [слот] размер файла изменился на newSize
    void OutputEventFileChanged(const QString &path, const int &oldSize, const int &newSize);



};

#endif // CONSOLELOGGER_H
