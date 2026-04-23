#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H


#include "ILogger.h"
#include <QDebug>


class ConsoleLogger : public ILogger {
public:
    ConsoleLogger() = default;
    ~ConsoleLogger() = default;

    // переопределение функции вывода - вывод на консоль QString data
    void Log(const QString &data);

};

#endif // CONSOLELOGGER_H
