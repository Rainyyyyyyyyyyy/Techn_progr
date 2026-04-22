#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
//#include <iostream>
#include <QString>

#include <thread>
#include <QDebug>

#include "FileMonitor.h"

#include "IException.h"

//using namespace std;

QTextStream cin(stdin);
//QTextStream cout(stdout);

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

    //QTextStream qout(stdout);        // для вывода
    QTextStream qin(stdin);             // для ввода

    QString path;
    //QTextStream stream;

    //QVector<QString> paths;
    // qDebug().noquote() ~ std::cout
    //unsigned int N = 0;

    do{
        qDebug()<<"Enter path to file-list: ";
        qin>>path;
        qin.flush();
    }
    while(!(QFileInfo(path).exists()));
    /*
     * E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\Lab1\ytr1.txt
     * E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\Lab1\ytr2.txt
     * E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\Lab1\ytr3.txt
     *
     * E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\Lab1\ListYtr.txt
     */
    /*
    for(unsigned int i=1; i<=N; i++){
        //qDebug().noquote()<<"Enter path to file[" + QString(QChar(i)) + "]: ";
        qDebug()<<"Enter path to file["<<i<<"]: "<<Qt::flush;
        //path = cin.readLine();
        qin>>path;
        qin.flush();
        // Проверка на существование только что введённого пути (файла по этому пути)
        if(!(QFileInfo::exists(path))){
            i--;
        }else{
            paths.push_back(path);
        }
    }
    */

    //IFileList *FileList1;// = new FileList(paths);
    IDelayer *Delayer1;// = new Delayer(1);
    //Logger *Logger1;// = new Logger();
    try{
        //FileList1 = new FileList(path);
        Delayer1 = new Delayer(1);
        //Logger1 = new Logger();

        /*if(FileList1 == NULL || FileList1 == nullptr){
            throw new ExceptionIFileListIsNull;
        }*/
        //if(Logger1 == NULL || Logger1 == nullptr){
        //    throw new ExceptionILoggerIsNull;
        //}
        //if(Delayer1 == NULL || Delayer1 == nullptr){
        //    throw new ExceptionIDelayerIsNull;
        //}
    }catch (CustomExceptions excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        //delete excp;
        return 1;
    }
    //FileMonitor FileMonitor1(FileList1, Logger1, Delayer1);
    FileMonitor FileMonitor1(path);
        //QObject::connect(&boss, &Employee::salaryChanged, PrintInfoSalaryA);
    //QObject::connect(&FileMonitor1, &FileMonitor::CheckStateOfFiles, )

    QObject::connect(&FileMonitor1, &FileMonitor::OnFileChange, &FileMonitor1, &FileMonitor::OutputEventFileChanged);
    QObject::connect(&FileMonitor1, &FileMonitor::OnFileExists, &FileMonitor1, &FileMonitor::OutputEventFileExists);
    QObject::connect(&FileMonitor1, &FileMonitor::OnFileLost, &FileMonitor1, &FileMonitor::OutputEventFileLost);
    try{
        while(true){
            FileMonitor1.CheckStateOfFiles();
            Delayer1->wait();
        }
    }catch (CustomExceptions excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        delete Delayer1;
        //delete excp;
        return 1;
    }

    return 0;
    //// E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\targetMonitoring1.txt
    ///



;
    return 0;
}
