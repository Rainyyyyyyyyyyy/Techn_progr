QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS_RELEASE = -O0  # Отключаем оптимизацию (во время работы в release режиме почему-то выбрасывается sigsegv в дизассемблировании)
QMAKE_CFLAGS_RELEASE = -O0

SOURCES += \
        src/ConsoleLogger.cpp \
        src/CustomException.cpp \
        src/Delayer.cpp \
        src/FileMonitor.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ILogger.h \
    ConsoleLogger.h \
    CustomException.h \
    IDelayer.h \
    Delayer.h \
    FileMonitorExceptions.h \
    FileMonitor.h
