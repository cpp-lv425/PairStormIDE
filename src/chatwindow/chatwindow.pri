DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/chatbase.h \
    $$PWD/chatwidget.h \
    $$PWD/chatwidgetinterface.h \
    $$PWD/chatwindowdock.h \
    $$PWD/messagesbackend.h \
    $$PWD/onlineuserslist.h \
    $$PWD/onlineusersmodel.h \
    $$PWD/qmlchatwidget.h

SOURCES += \
    $$PWD/chatwidget.cpp \
    $$PWD/chatwindowdock.cpp \
    $$PWD/messagesbackend.cpp \
    $$PWD/onlineuserslist.cpp \
    $$PWD/onlineusersmodel.cpp \
    $$PWD/qmlchatwidget.cpp

include($$PWD/messagesmodel/messagesmodel.pri)

RESOURCES += \
    $$PWD/qmlchat.qrc
