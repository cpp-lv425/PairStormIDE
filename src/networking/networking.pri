DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

SOURCES += \
        $$PWD/apiparser.cpp \
        $$PWD/connectionserviceprovider.cpp \
        $$PWD/tcpservice.cpp \
        $$PWD/udpservice.cpp

HEADERS += \
    $$PWD/apiparser.h \
    $$PWD/connectionserviceprovider.h \
    $$PWD/networkbase.h \
    $$PWD/tcpservice.h \
    $$PWD/udpservice.h

#include($$PWD/changesManager/changesmanager.pri)
