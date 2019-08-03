DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/browser.h \
    $$PWD/browserdialog.h \

SOURCES += \
    $$PWD/browser.cpp \
    $$PWD/browserdialog.cpp \

FORMS += \
    $$PWD/browserdialog.ui

include($$PWD/documentationengine/documentationengine.pri)
