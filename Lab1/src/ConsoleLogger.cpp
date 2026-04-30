#include "ConsoleLogger.h"

#include <QDebug>

// переопределение Log() от базового класса
void ConsoleLogger::Log(QString s) {
    qDebug()<<s;
}



// [слот] файл существует
void ConsoleLogger::OutputEventFileExists(const QString &path, const int &currentSize) {
    Log(path + " --- File is exists. Size: " + QString::number(currentSize) + " bytes.");
}

// [слот] файл удалён, перемещён или переименован
void ConsoleLogger::OutputEventFileLost(const QString &path) {
    Log(path + " --- File has been deleted, replaced or renamed.");
}

// [слот] размер файла изменился на newSize
void ConsoleLogger::OutputEventFileChanged(const QString &path, const int &oldSize, const int &newSize) {
    Log(path + " --- Size has been changed. Size:  " + QString::number(oldSize) + " -> " + QString::number(newSize) + " bytes.");
}


// определение функции вывода - вывод на консоль QString data
/*
// Тестирование
на вход: "message" - выход: вывод на консоль "message\n"
на вход: "message\n" - выход: вывод на консоль "message\n\n"
на вход: QString message = "var_message" - выход: вывод на консоль "var_message\n"
на вход: "" - выход: вывод на косноль "\n"
на вход: QString message = "" - выход: вывод на консоль "\n"
*/
