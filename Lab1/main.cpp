#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
//#include <iostream>
#include <QString>

#include <thread>
#include <QDebug>

#include "FileMonitor.h"
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
    QTextStream qout(stdout);        // для вывода
    QTextStream qin(stdin);             // для ввода

    QString path;
    QTextStream stream;

    QVector<QString> paths;
    // qDebug().noquote() ~ std::cout
    unsigned int N = 0;
    //qDebug().quote()<<"Enter amount of files (for start): ";
    // Qt::flush() в конце необходим
    qout<<"Enter amount of files (for start): "<<Qt::flush;
    qin>>N;
    qin.flush();
    /*
     * E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\Lab1\ytr1.txt
     * E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\Lab1\ytr2.txt
     * E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\Lab1\ytr3.txt
     */
    for(unsigned int i=1; i<=N; i++){
        //qDebug().noquote()<<"Enter path to file[" + QString(QChar(i)) + "]: ";
        qout<<"Enter path to file["<<i<<"]: "<<Qt::flush;
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
    //// E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\targetMonitoring1.txt
    ///
    /*QFileInfo fileinfo;     //(path);
    do{
        //qDebug().noquote()<<"Error: not a file.\n";

        //qDebug().noquote()<<"Enter path to file: ";
        path = cin.readLine();
        fileinfo = QFileInfo(path);
        //cout.flush();
        // H:\Documents
    }while(!fileinfo.isFile());*/

    FileMonitor Monitorer(paths);
    while(true){
        //fileinfo.refresh();
        Monitorer.CheckStateOfFiles();

        //qDebug().noquote()<<"File exists: "<<fileinfo.exists();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    //qDebug().noquote();



    //QCoreApplication a(argc, argv);

    //return a.exec();
    //system("pause");
    return 0;
}
