#include <QCoreApplication>
#include <QFileInfo>
#include <QTextStream>
#include <QString>

#include <thread>
#include <QDebug>

#include "FileMonitor.h"
#include "Delayer.h"
#include "CustomException.h"

#include <windows.h>
#include <sys/stat.h>
// анонимное namespace известное этому и только этому файлу (FolderTraveler.cpp)
namespace {
/*
 * Windows:
 *      системный файл ((GetFileAttributes() & FILE_ATTRIBUTE_SYSTEM) == true) -> return true
 *      несистемный файл ((GetFileAttributes() & FILE_ATTRIBUTE_SYSTEM) == false) -> return false
 * Unix:
 *      файл в одной из подсистемных папок:
 *      /proc
 *      /sys
 *      /dev
 *      /run
 *      либо содержит в полном пути эти папки -> return true
 *      файл вне подсистемных папок:
 *       /proc
 *      /sys
 *      /dev
 *      /run
 *      и не содержит в полном пути этих папок -> return false
*/
bool isSystemEntry(const QFileInfo &entry){
#ifdef Q_OS_WIN
    const std::wstring path = entry.absoluteFilePath().toStdWString();
    const DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return (attrs & FILE_ATTRIBUTE_SYSTEM) != 0;
#elif defined(Q_OS_UNIX)
    const QString p = entry.absoluteFilePath();
    if (p == "/proc" || p == "/sys" || p == "/dev" || p == "/run" ||
        p.startsWith("/proc/") || p.startsWith("/sys/") ||
        p.startsWith("/dev/") || p.startsWith("/run/")) {
        return true;
    }

    struct stat st {};
    const QByteArray nativePath = p.toLocal8Bit();
    if (::stat(nativePath.constData(), &st) != 0) {
        return false;
    }

    // Не добавляем специальные узлы ФС (device/fifo/socket).
    return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode) ||
           S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode);
#else
    Q_UNUSED(entry);
    return false;
#endif
}

}




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



bool ReadHostFile(QString path, QVector <QString> &monitorpaths){
    if(!path.endsWith(".txt")){
        qDebug()<<"<from UI> Incorrect input: hostfile format is not .txt.";
        return false;
    }
    QFileInfo fileinfo(path);
    fileinfo.refresh();
    if(!fileinfo.exists()){
        qDebug()<<"<from UI> Incorrect input: file does not exists.";
        return false;
    }
    if(fileinfo.isHidden() || isSystemEntry(path)){
        qDebug()<<"<from UI> Incorrect input: file is hidden or from system-space.";
        return false;
    }

    monitorpaths.clear();
    QFile File_with_List(path);
        if (File_with_List.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream File_content(&File_with_List);

            while(!File_content.atEnd()){
                QString temp_path = File_content.readLine();
                if(!temp_path.isEmpty() && !monitorpaths.contains(temp_path)){
                    monitorpaths.append(temp_path);
                }
            }
            File_with_List.close();
            if(monitorpaths.empty()){
                qDebug()<<"<from UI> Warning: hostfile is empty!";
                return false;
            }
        }else{
            qDebug()<<"<from UI> Unable to open hostfile.";
            return false;
        }
        return true;
}


bool checkAction(QString act, QVector <QString> possibleActs){
        return possibleActs.contains(act);
}


QTextStream qin(stdin);             // для ввода


void askAction(QString &act){
    QVector <QString> Actions = {".reset", ".exit", ".continue"};
    do{
        qDebug()<<"Enter action ('.reset', '.continue', '.exit'): ";
        qin>>act;
        if(checkAction(act, Actions) == false){ qDebug()<<"Not resolved actions. Default = '.reset'"; act = ".reset"; }
    }while(checkAction(act, Actions) == false);
}

int main(int argc, char *argv[])
{


/*
        QCoreApplication qcoreappa(argc, argv);
        qDebug()<<"Current relative path: "<<QCoreApplication::applicationFilePath()<<Qt::endl;

        // path to host-file
        QString path;
        QString act = ".reset";
        QString pathh = path;//"../TestCases/tt/HostFile.txt";
        QVector <QString> paths;
        QVector <QString> Actions = {".reset", ".exit", ".continue"};
        do{
            qDebug()<<"Enter path to file-list: ";
            qin>>path;
            if(ReadHostFile(path, paths) == false){
                act = ".reset";
                continue;
            }
            askAction(act);
        }while(act == ".reset");

        if(act == ".exit"){
            qDebug()<<"Exit...";
            return 0;
        }
        act = ".reset";


    // Наблюдатель, для него необходима сущность-вывод (ILogger, ConsoleLogger)
    FileMonitor FileMonitor11;
    Delayer delayer11;
    ConsoleLogger logger11;


    try{ FileMonitor11.Init(paths, &logger11); }
    catch (CustomExceptions &excp){
        qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
        return 1;
    }
    try{
        while(true){
            FileMonitor11.CheckStateOfFiles();
            if(ReadHostFile(path, paths) == false){
                    qDebug()<<"exit? ('.exit', 'continue'): ";
            }
            FileMonitor11.refreshList(paths);

            delayer11.wait();
        }
    }catch (CustomExceptions &excp){
            qDebug()<<(excp.what())<<"  Code: "<<excp.getCode();
            return 1;
    }
    */


    /// =======       TEST-CASES      ======== ///
    /** === P0 === **/
    /* testcase 0.1 */ /*
    QString path_test = "../TestCases/P0/1/HostFIle.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 0.2 */ /*
    QString path_test = "../TestCases/P0/2/";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 0.3 */ /*
    QString path_test = "../TestCases/P0/3/HostFile.dat";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */


    /** === [P1] === **/
    /* testcase 1 */ /*
    QString path_test = "../TestCases/P1/1/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 2*/ /*
    QString path_test = "../TestCases/P1/2/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 3 */ /*
    QString path_test = "../TestCases/P1/3/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */


    /** === [P2] === **/
    /* testcase 1 */ /*
    QString path_test = "../TestCases/P2/1/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 2 */ /*
    QString path_test = "../TestCases/P2/2/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 3 */ /*
    QString path_test = "../TestCases/P2/3/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 4 */ /*
    QString path_test = "../TestCases/P2/4/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 5 */ /*
    QString path_test = "../TestCases/P2/5/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 6 */ /*
    QString path_test = "../TestCases/P2/6/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 7 */ /*
    QString path_test = "../TestCases/P2/7/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */

    /* testcase 8 */ /*
    QString path_test = "../TestCases/P2/8/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    return 0;
    */


    /** === [P3] === **/
    /** testcase 1-8 последовательно проверить вручную **/ /*
    QString path_test = "../TestCases/P3/1/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    Delayer delayer_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    while(true){
        monitor_test.CheckStateOfFiles();
        if(ReadHostFile(path_test, paths_test) == false){
                qDebug()<<"exit? ('.exit', 'continue'): ";
        }
        monitor_test.refreshList(paths_test);
        delayer_test.wait();
    }
    return 0;
    */


    /** === [P4] === **/
    /** testcase 1-9 последовательно проверить вручную **/ /*
    QString path_test = "../TestCases/P4/1/HostFile.txt";
    QVector <QString> paths_test;
    qDebug()<<"ReadHostFile(): "<<ReadHostFile(path_test, paths_test);
    FileMonitor monitor_test;
    ConsoleLogger logger_test;
    Delayer delayer_test;
    qDebug()<<"Init(): "<<monitor_test.Init(paths_test, &logger_test);
    qDebug()<<"getList(): ";
    QList <QString> readen_list = monitor_test.getList();
    for(QString i : readen_list){ qDebug()<<i; }
    while(true){
        monitor_test.CheckStateOfFiles();
        if(ReadHostFile(path_test, paths_test) == false){
                qDebug()<<"exit? ('.exit', 'continue'): ";
        }
        monitor_test.refreshList(paths_test);
        delayer_test.wait();
    }
    return 0;
    */










    return 0;
}
