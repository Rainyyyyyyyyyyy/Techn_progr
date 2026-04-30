#ifndef ILOGGER_H
#define ILOGGER_H

#endif // ILOGGER_H


#include <QString>
#include <QObject>

#include <QDebug>

class ILogger : public QObject
{
       Q_OBJECT

public:

       virtual ~ILogger() {}

    // функция вывода
public slots:
       virtual void Log(QString) = 0;

};


