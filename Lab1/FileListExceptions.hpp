#ifndef FILELISTEXCEPTIONS_HPP
#define FILELISTEXCEPTIONS_HPP


#include "IException.h"

// коды возможных ошибок(исключений)
#define EXCEPTION_UNABLE_TO_OPEN_FILE 101
#define EXCEPTION_INDEX_OUT_OF_BOUNDS 102
#define EXCEPTION_FILEPATH_TO_LIST_IS_EMPTY 103

//std::exception *excpc = new std::exception;



/* Исключение: невозможно открыть файл */
class ExceptionUnableToOpenFile : public CustomExceptions{//Exceptions {
public:
    ExceptionUnableToOpenFile() : CustomExceptions(EXCEPTION_UNABLE_TO_OPEN_FILE, "Unable to open file!"){}
};


/* Исключение: выход за пределы массива */
class ExceptionIndexOutOfBounds : public CustomExceptions {
public:
    ExceptionIndexOutOfBounds() : CustomExceptions(EXCEPTION_INDEX_OUT_OF_BOUNDS, "Error: index out of bounds!"){}
};

/* Исключение: путь к файлу-списку пуст */
class ExceptionFilePathToListIsEmpty : public CustomExceptions {
public:
    ExceptionFilePathToListIsEmpty() : CustomExceptions(EXCEPTION_FILEPATH_TO_LIST_IS_EMPTY, "Error: Path to file-list is empty!"){}
};


#endif // FILELISTEXCEPTIONS_HPP
