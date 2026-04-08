#ifndef FILEMONITOREXCEPTIONS_H
#define FILEMONITOREXCEPTIONS_H

#include "IException.h"


#define EXCEPTION_IFILELIST_IS_NULLPTR 102
#define EXCEPTION_ILOGGER_IS_NULLPTR 103
#define EXCEPTION_IDELAYER_IS_NULLPTR 104
#define EXCEPTION_FILELIST_IS_EMPTY 105

/* Исключение: IFileList == nullptr || IFileList == NULL */
class ExceptionIFileListIsNull : public Exceptions {
public:
    ExceptionIFileListIsNull() : Exceptions(EXCEPTION_IFILELIST_IS_NULLPTR){}

    const char * what() const noexcept override{
        return "Error: IFileList * is nullptr (or NULL)!";
    }
};


/* Исключение: ILogger == nullptr || ILogger == NULL */
class ExceptionILoggerIsNull : public Exceptions {
public:
    ExceptionILoggerIsNull() : Exceptions(EXCEPTION_ILOGGER_IS_NULLPTR){}

    const char * what() const noexcept override{
        return "Error: ILogger * is nullptr (or NULL)!";
    }
};

/* Исключение: ILogger == nullptr || ILogger == NULL */
class ExceptionIDelayerIsNull : public Exceptions {
public:
    ExceptionIDelayerIsNull() : Exceptions(EXCEPTION_IDELAYER_IS_NULLPTR){}

    const char * what() const noexcept override{
        return "Error: IDelayer * is nullptr (or NULL)!";
    }
};

/* Исключение: FileList пуст (для CheckStateOfFiles) */
class ExceptionFileListIsEmpty : public Exceptions {
public:
    ExceptionFileListIsEmpty() : Exceptions(EXCEPTION_FILELIST_IS_EMPTY){}

    const char * what() const noexcept override{
        return "Error: Empty Filelist!";
    }
};


#endif // FILEMONITOREXCEPTIONS_H
