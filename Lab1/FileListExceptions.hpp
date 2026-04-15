#ifndef FILELISTEXCEPTIONS_HPP
#define FILELISTEXCEPTIONS_HPP


#include "IException.h"

// коды возможных ошибок(исключений)
#define EXCEPTION_UNABLE_TO_OPEN_FILE 101
#define EXCEPTION_INDEX_OUT_OF_BOUNDS 102
#define EXCEPTION_FILEPATH_TO_LIST_IS_EMPTY 103




/* Исключение: невозможно открыть файл */
class ExceptionUnableToOpenFile : public Exceptions {
public:
    ExceptionUnableToOpenFile() : Exceptions(EXCEPTION_UNABLE_TO_OPEN_FILE){}

    const char * what() const noexcept override{
        return "Unable to open file!";
    }
};


/* Исключение: выход за пределы массива */
class ExceptionIndexOutOfBounds : public Exceptions {
public:
    ExceptionIndexOutOfBounds() : Exceptions(EXCEPTION_INDEX_OUT_OF_BOUNDS){}

    const char * what() const noexcept override{
        return "Error: index out of bounds!";
    }
};

/* Исключение: путь к файлу-списку пуст */
class ExceptionFilePathToListIsEmpty : public Exceptions {
public:
    ExceptionFilePathToListIsEmpty() : Exceptions(EXCEPTION_FILEPATH_TO_LIST_IS_EMPTY){}

    const char * what() const noexcept override{
        return "Error: Path to file-list is empty!";
    }
};


#endif // FILELISTEXCEPTIONS_HPP
