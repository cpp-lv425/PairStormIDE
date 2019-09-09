DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD


HEADERS += \
    $$PWD/bottompaneldock.h \
    $$PWD/compilewindow.h

SOURCES += \
    $$PWD/bottompaneldock.cpp \
    $$PWD/compilewindow.cpp

include($$PWD/consolewindow/consolewindow.pri)

FORMS += \
    $$PWD/compilewindow.ui
