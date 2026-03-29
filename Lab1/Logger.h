#ifndef LOGGER_H
#define LOGGER_H

#endif // LOGGER_H

#include "ILogger.h"
#include <QDebug>

class Logger : public ILogger {
public:
    ~Logger() = default;

    // определение функции вывода - вывод на консоль QString data
    void Log(const QString &data);


};
