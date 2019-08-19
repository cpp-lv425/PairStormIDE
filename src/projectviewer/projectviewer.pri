DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

SOURCES += \
        $$PWD/projectviewerdock.cpp

HEADERS += \
    $$PWD/projectviewerdock.h

include($$PWD/projectviewermodel/projectviewermodel.pri)
include($$PWD/projecttreeview/projecttreeview.pri)
