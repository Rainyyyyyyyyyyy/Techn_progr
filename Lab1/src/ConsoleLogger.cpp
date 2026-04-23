#include "ConsoleLogger.h"



// определение функции вывода - вывод на консоль QString data
void ConsoleLogger::Log(const QString &data){
    // qDebug() << выводит в конце '\n' автоматически
    qDebug()<<"Log_data: "<<data<<Qt::flush;
}

/*
// Тестирование
на вход: "message" - выход: вывод на консоль "message\n"
на вход: "message\n" - выход: вывод на консоль "message\n\n"
на вход: QString message = "var_message" - выход: вывод на консоль "var_message\n"
на вход: "" - выход: вывод на косноль "\n"
на вход: QString message = "" - выход: вывод на консоль "\n"
*/
