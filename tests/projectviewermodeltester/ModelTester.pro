QT += testlib core widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_projectviewermodel.cpp

SOURCES += \
    tst_projectviewermodel.cpp \


include($$PWD/../../src/projectviewermodel/projectviewermodel.pri)
