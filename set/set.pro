QT -= gui

TEMPLATE = lib
DEFINES += SET_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/MySet.cpp

LIBS += -L$$PWD/libs/ -llogger -L$$PWD/libs/ -lvector

HEADERS += \
    include/ISet.h \
    include/RC.h \
    set_global.h \
    src/ILogger.h \
    src/IVector.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
