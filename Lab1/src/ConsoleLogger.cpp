#include "ConsoleLogger.h"

#include <QDebug>

// переопределение Log() от базового класса
void ConsoleLogger::Log(QString s) {
    qDebug()<<s;
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
