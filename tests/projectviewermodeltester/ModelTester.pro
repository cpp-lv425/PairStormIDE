QT += testlib core widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  \
    tst.cpp

SOURCES += \


include($$PWD/../../src/projectviewer/projectviewermodel/projectviewermodel.pri)
