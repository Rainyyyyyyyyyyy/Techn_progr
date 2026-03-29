#ifndef LOGGER_H
#define LOGGER_H

#endif // LOGGER_H

#include "ILogger.h"
#include <QDebug>

class Logger : public ILogger {
public:
    ~Logger() = default;

    void Log(QString data){
        qDebug()<<"Log_data: "<<data;
    }

};
