DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/chatwidget.h \
    $$PWD/chatwidgetinterface.h \
    $$PWD/chatwindowdock.h \
    $$PWD/qmlchatwidget.h

SOURCES += \
    $$PWD/chatwidget.cpp \
    $$PWD/chatwindowdock.cpp \
    $$PWD/qmlchatwidget.cpp

include($$PWD/messagesmodel/messagesmodel.pri)
include($$PWD/usersmodel/usersmodel.pri)

RESOURCES += \
    $$PWD/qmlchat.qrc
