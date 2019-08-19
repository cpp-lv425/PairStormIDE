DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/browserdialog.h \

SOURCES += \
    $$PWD/browserdialog.cpp \

FORMS += \
    $$PWD/browserdialog.ui

include($$PWD/documentationengine/documentationengine.pri)
