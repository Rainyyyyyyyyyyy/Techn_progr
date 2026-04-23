#include "CustomException.h"

// конструктор по значениям
CustomExceptions::CustomExceptions(unsigned int code__, const char * message__) : errorCode(code__), message(message__) {};

// пеергрузка const char * std::exception::what()
const char * CustomExceptions::what() const noexcept { return message.c_str(); }

    // getter кода ошибки/исключения
unsigned int CustomExceptions::getCode() const { return errorCode; }
