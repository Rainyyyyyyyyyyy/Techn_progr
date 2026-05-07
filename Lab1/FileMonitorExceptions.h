#ifndef FILEMONITOREXCEPTIONS_H
#define FILEMONITOREXCEPTIONS_H

#include "CustomException.h"



#define EXCEPTION_FILELIST_IS_EMPTY 105
#define EXCEPTION_FILEPATH_IN_LIST_IS_EMPTY 106
#define EXCEPTION_UNABLE_TO_OPEN_FILE 107
#define EXCEPTION_DOT_OR_DOTDOT_IN_PATH_TO_HOST_FILE 108
#define EXCEPTION_DOT_OR_DOTDOT_IN_PATH 1089
#define EXCEPTION_DOT_OR_DOTDOT_IN_PATH_TO_CHECKING_FILE 109
#define EXCEPTION_FILE_IS_HIDDEN 110


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

/* Исключение: '.' или '..' в пути к хост-файлу */
class ExceptionDotOrDotDotInHostPath : public CustomExceptions {
public:
    ExceptionDotOrDotDotInHostPath() : CustomExceptions(EXCEPTION_DOT_OR_DOTDOT_IN_PATH_TO_HOST_FILE, "Warning! '.' or '..' in path to host-file!"){}
};


/* Исключение: '.' или '..' внутри хост-файла (путь к файлу для наблюдения содержит '.' или '..') */
class ExceptionDotOrDotDotInListToCheck : public CustomExceptions {
public:
    ExceptionDotOrDotDotInListToCheck() : CustomExceptions(EXCEPTION_DOT_OR_DOTDOT_IN_PATH_TO_CHECKING_FILE, "Warning! '.' or '..' in path from host-file!"){}
};

/* Исключение: файл является скрытым (на Windows) */
class ExceptionFileIsHidden : public CustomExceptions {
public:
    ExceptionFileIsHidden() : CustomExceptions(EXCEPTION_FILE_IS_HIDDEN, "Error: file is hidden!"){}
};

/* Исключение: файл является скрытым (на Windows) */
class ExceptionDotOrDotDotInPath : public CustomExceptions {
public:
    ExceptionDotOrDotDotInPath() : CustomExceptions(EXCEPTION_DOT_OR_DOTDOT_IN_PATH, "Warning! '.' or '..' in path!"){}
};

#endif // FILEMONITOREXCEPTIONS_H
