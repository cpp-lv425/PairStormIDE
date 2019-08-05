DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/connectionmanager.h \
    $$PWD/constantsdocumentationl.h \
    $$PWD/documentationengine.h \
    $$PWD/documentationsearch.h \
    $$PWD/documentationviewer.h \
    $$PWD/htmlcontentgenerator.h \
    $$PWD/mustache.hpp

SOURCES += \
    $$PWD/connectionmanager.cpp \
    $$PWD/documentationengine.cpp \
    $$PWD/documentationsearch.cpp \
    $$PWD/documentationviewer.cpp \
    $$PWD/htmlcontentgenerator.cpp
