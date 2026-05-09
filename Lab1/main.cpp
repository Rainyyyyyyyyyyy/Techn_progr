#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
#include <QString>

#include <thread>
#include <QDebug>

#include "FileMonitor.h"
#include "Delayer.h"
#include "CustomException.h"




/*
 * Существует несколько ситуаций для наблюдаемого файла:

1. Файл существует, файл не  пустой - на
    экран выводится факт существования файла и его размер.
2. Файл существует, файл был изменен - на
    экран выводится факт существования файла,
    сообщение о том что файл был изменен и его размер.
3. Файл не существует - на экран выводится
    информация о том что файл не существует.


При возникновении изменения состояния наблюдаемого
 файла ( возникновение события ),
необходимо выводить на экран соответствующее сообщение.

В данной реализации используем механизм
сигнально-слотового соединения для обеспечения
обработки события изменения наблюдаемого файла.
*/
int main(int argc, char *argv[])
{
    QTextStream qin(stdin);             // для ввода
    QCoreApplication qcoreappa(argc, argv);
    qDebug()<<"Current relative path: "<<QCoreApplication::applicationFilePath()<<Qt::endl;

    // path to host-file
    QString path;
    qDebug()<<"Enter path to file-list: ";
    qin>>path;



    // Наблюдатель, для него необходима сущность-вывод (ILogger, ConsoleLogger)
    FileMonitor FileMonitor1;
    Delayer delayer1;
    ConsoleLogger logger1;

    try{
        FileMonitor1.Init(path, &logger1);
    }catch (CustomExceptions &excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        return 1;
    }


    try{
        while(true){
            FileMonitor1.CheckStateOfFiles();
            delayer1.wait();
        }
    }catch (CustomExceptions &excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        return 1;
    }

    return 0;
}
