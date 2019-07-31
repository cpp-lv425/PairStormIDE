DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

SOURCES += \
        $$PWD/localconnector.cpp \
        $$PWD/localconnectorgenerator.cpp \
        $$PWD/tcpservice.cpp \
        $$PWD/udpservice.cpp

HEADERS += \
    $$PWD/localconnector.h \
    $$PWD/localconnectorgenerator.h \
    $$PWD/networkbase.h \
    $$PWD/tcpservice.h \
    $$PWD/udpservice.h

#include($$PWD/changesManager/changesmanager.pri)
