#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H


#include "ILogger.h"
#include <QDebug>


class ConsoleLogger : public ILogger{
public:

    ConsoleLogger() {};
    ~ConsoleLogger() {};

    // переопределение log от базового класса
    void Log(QString s) override;

};

#endif // CONSOLELOGGER_H
