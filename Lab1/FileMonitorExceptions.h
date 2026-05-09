#ifndef FILEMONITOREXCEPTIONS_H
#define FILEMONITOREXCEPTIONS_H

#include "CustomException.h"



#define EXCEPTION_FILELIST_IS_EMPTY 105
#define EXCEPTION_UNABLE_TO_OPEN_FILE 106
#define EXCEPTION_FILE_IS_HIDDEN 107
#define EXCEPTION_HOSTFILE_IS_HIDDEN 108
#define EXCEPTION_INCORRECT_FORMAT_OF_HOSTFILE 109
#define EXCEPTION_PATH_TO_FILE_IS_PATH_TO_DIR 110



/* Исключение: FileList пуст (для CheckStateOfFiles) */
class ExceptionFileListIsEmpty : public CustomExceptions {
public:
    ExceptionFileListIsEmpty() : CustomExceptions(EXCEPTION_FILELIST_IS_EMPTY, "Error: Empty list-of-files!"){}
};

/* Исключение: не удалось открыть файл */
class ExceptionUnableToOpenFile : public CustomExceptions {
public:
    ExceptionUnableToOpenFile() : CustomExceptions(EXCEPTION_UNABLE_TO_OPEN_FILE, "Error: unable to open file"){}
};

/* Исключение: файл является скрытым (на Windows) */
class ExceptionFileIsHidden : public CustomExceptions {
public:
    ExceptionFileIsHidden() : CustomExceptions(EXCEPTION_FILE_IS_HIDDEN, "Warning! File is hidden!"){}
};

/* Исключение: хост-файл является скрытым (на Windows) */
class ExceptionHostFileIsHidden : public CustomExceptions {
public:
    ExceptionHostFileIsHidden() : CustomExceptions(EXCEPTION_HOSTFILE_IS_HIDDEN, "Warning! Host-file is hidden!"){}
};

/* Исключение: неверный формат хост-файла */
class ExceptionIncorrectFormatOfHostfile : public CustomExceptions {
public:
    ExceptionIncorrectFormatOfHostfile() : CustomExceptions(EXCEPTION_INCORRECT_FORMAT_OF_HOSTFILE, "Warning! Incorrect format of hostfile!"){}
};

/* Исключение: неверный формат хост-файла */
class ExceptionFilePathIsDirPath : public CustomExceptions {
public:
    ExceptionFilePathIsDirPath() : CustomExceptions(EXCEPTION_PATH_TO_FILE_IS_PATH_TO_DIR, "Warning! Got path to directory, but path to file excepted!"){}
};


#endif // FILEMONITOREXCEPTIONS_H
