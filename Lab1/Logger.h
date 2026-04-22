#ifndef LOGGER_H
#define LOGGER_H

#endif // LOGGER_H

#include "ILogger.h"
#include <QDebug>

class ConsoleLogger : public ILogger {
public:
    ConsoleLogger() = default;
    ~ConsoleLogger() = default;

    // определение функции вывода - вывод на консоль QString data
    void Log(const QString &data);


};
