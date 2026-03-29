#ifndef ILOGGER_H
#define ILOGGER_H

#endif // ILOGGER_H


#include <QString>

class ILogger
{
public:

    virtual ~ILogger() = default;

    // функция вывода
    virtual void Log(const QString &data) = 0;
};


