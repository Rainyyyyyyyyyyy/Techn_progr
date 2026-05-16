#ifndef FILEMONITOREXCEPTIONS_H
#define FILEMONITOREXCEPTIONS_H

#include "CustomException.h"


#define EXCEPTION_FILE_IS_HIDDEN 101
#define EXCEPTION_PATH_TO_FILE_IS_PATH_TO_DIR 102
#define EXCPTION_FILE_IS_FROM_SYSTEM_SPACE 103


/* Исключение: файл является скрытым (на Windows) */
class ExceptionFileIsHidden : public CustomExceptions {
public:
    ExceptionFileIsHidden() : CustomExceptions(EXCEPTION_FILE_IS_HIDDEN, "Warning! File is hidden!"){}
};

/* Исключение: путь к файлу является путём к папке */
class ExceptionFilePathIsDirPath : public CustomExceptions {
public:
    ExceptionFilePathIsDirPath() : CustomExceptions(EXCEPTION_PATH_TO_FILE_IS_PATH_TO_DIR, "Warning! Got path to directory, but path to file excepted!"){}
};

/* Исключение: получен путь к файлу из системмного пространства */
class ExceptionFileIsFromSystemSpace : public CustomExceptions {
public:
    ExceptionFileIsFromSystemSpace() : CustomExceptions(EXCPTION_FILE_IS_FROM_SYSTEM_SPACE, "Warning! Got path from system space"){}
}; //


#endif // FILEMONITOREXCEPTIONS_H
