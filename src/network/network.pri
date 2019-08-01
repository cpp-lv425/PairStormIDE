DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

SOURCES += \
        $$PWD/defaultlocalconnector.cpp \
        $$PWD/localconnectorgenerator.cpp \
        $$PWD/tcpservice.cpp \
        $$PWD/udpservice.cpp

HEADERS += \
    $$PWD/defaultlocalconnector.h \
    $$PWD/localconnectorgenerator.h \
    $$PWD/localconnectorinterface.h \
    $$PWD/networkbase.h \
    $$PWD/tcpservice.h \
    $$PWD/udpservice.h

#include($$PWD/changesManager/changesmanager.pri)
