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
    CustomExceptions(unsigned int code__, const char * message__);
    // переопределение const char * std::exception::what()
    const char *what() const noexcept override;
    // getter кода ошибки/исключения
    unsigned int getCode() const;
};


#endif // CUSTOMEXCEPTION_H
