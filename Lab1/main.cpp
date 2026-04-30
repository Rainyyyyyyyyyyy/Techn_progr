#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
//#include <iostream>
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
    QString path;
    do{
        qDebug()<<"Enter path to file-list: ";
        qin>>path;
        qin.flush();
    }
    while(!(QFileInfo::exists(path)));


    IDelayer *Delayer1;// = new Delayer(1);
    try{
        Delayer1 = new Delayer(1);
    }catch (CustomExceptions excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        return 1;
    }
    ILogger *__logger = new ConsoleLogger;

    FileMonitor FileMonitor1;


    //QObject::connect(&FileMonitor1, &FileMonitor::OnFileChange, &FileMonitor1, &FileMonitor::OutputEventFileChanged);
    //QObject::connect(&FileMonitor1, &FileMonitor::OnFileExists,   &FileMonitor1, &FileMonitor::OutputEventFileExists);
    //QObject::connect(&FileMonitor1, &FileMonitor::OnFileLost,     &FileMonitor1, &FileMonitor::OutputEventFileLost);



    try{
        FileMonitor1.Init(path, __logger);
    }catch (CustomExceptions &excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        delete Delayer1;
        delete __logger;
        return 1;
    }


    try{
        while(true){
            FileMonitor1.CheckStateOfFiles();
            Delayer1->wait();
        }
    }catch (CustomExceptions &excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        delete Delayer1;
        delete __logger;
        return 1;
    }

    return 0;
}


/*
     *
     * E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\Lab1\ListYtr.txt
     */
