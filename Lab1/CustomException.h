#ifndef CUSTOMEXCEPTION_H
#define CUSTOMEXCEPTION_H


#include <exception>    // из std
#include <string>


class CustomExceptions : public std::exception{
protected:
    unsigned int errorCode;           // код ошибки\исключения
    std::string message;    // описание
public:
    // конструктор по значениям
    CustomExceptions(unsigned int code__=0, const char * message__ = "Unexpected error.") : errorCode(code__), message(message__) {};
    // переопределение const char * std::exception::what()
    const char *what() const noexcept override {return message.c_str(); }
    // getter кода ошибки/исключения
    unsigned int getCode() const { return errorCode; }
};


#endif // CUSTOMEXCEPTION_H
