#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
#include <iostream>
#include <QString>





//using namespace std;

QTextStream cin(stdin);
QTextStream cout(stdout);


int main(int argc, char *argv[])
{
    QString path;
    QTextStream stream;


    cout<<"Enter path to file: ";
    cout.flush();

    path = cin.readLine();
    QFileInfo file(path);

    if(file.isDir()){
        cout<<"Error: not a file.\n"<<Qt::endl;
        //cout.flush();
        // H:\Documents
    }

    cout.flush();



    //QCoreApplication a(argc, argv);

    //return a.exec();
    //system("pause");
        return 0;
}
