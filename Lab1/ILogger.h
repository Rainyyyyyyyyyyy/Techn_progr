#ifndef ILOGGER_H
#define ILOGGER_H

#endif // ILOGGER_H


#include <QString>

class ILogger
{
public:

    virtual ~ILogger() = default;
    virtual void Log(QString data) = 0;
};


