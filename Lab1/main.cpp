#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
#include <iostream>
#include <QString>

#include <thread>
#include <QDebug>


//using namespace std;

QTextStream cin(stdin);
//QTextStream cout(stdout);


int main(int argc, char *argv[])
{
    QString path;
    QTextStream stream;


    qDebug().noquote()<<"Enter path to file: ";
    //cout.flush();

    path = cin.readLine();
    //// E:\Z_vsyakoe_dla_echeby\4k2sem\Technologii_Programmirovania(Andreeva)\laba1\Qt\try1(git_clone)\Techn_progr\targetMonitoring1.txt
    ///
    QFileInfo fileinfo;     //(path);
    do{
        qDebug().noquote()<<"Error: not a file.\n";

        qDebug().noquote()<<"Enter path to file: ";
        path = cin.readLine();
        fileinfo = QFileInfo(path);
        //cout.flush();
        // H:\Documents
    }while(!fileinfo.isFile());
    while(true){
        fileinfo.refresh();
        qDebug().noquote()<<"File exists: "<<fileinfo.exists();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    //qDebug().noquote();



    //QCoreApplication a(argc, argv);

    //return a.exec();
    //system("pause");
    return 0;
}
