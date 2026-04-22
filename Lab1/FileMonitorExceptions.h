#ifndef FILEMONITOREXCEPTIONS_H
#define FILEMONITOREXCEPTIONS_H

#include "IException.h"



#define EXCEPTION_FILELIST_IS_EMPTY 105
#define EXCEPTION_FILEPATH_IN_LIST_IS_EMPTY 106
#define EXCEPTION_UNABLE_TO_OPEN_FILE 107



/* Исключение: FileList пуст (для CheckStateOfFiles) */
class ExceptionFileListIsEmpty : public CustomExceptions {
public:
    ExceptionFileListIsEmpty() : CustomExceptions(EXCEPTION_FILELIST_IS_EMPTY, "Error: Empty list-of-files!"){}
};

/* Исключение: в FileList попался пустой путь */
class ExceptionFilePathInFileListIsEmpty : public CustomExceptions {
public:
    ExceptionFilePathInFileListIsEmpty() : CustomExceptions(EXCEPTION_FILEPATH_IN_LIST_IS_EMPTY, "Error: Empty FilePath in list-of-files"){}
};

/* Исключение: не удалось открыть файл */
class ExceptionUnableToOpenFile : public CustomExceptions {
public:
    ExceptionUnableToOpenFile() : CustomExceptions(EXCEPTION_UNABLE_TO_OPEN_FILE, "Error: unable to open file"){}
};

#endif // FILEMONITOREXCEPTIONS_H
