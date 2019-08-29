#ifndef QMLCHATWIDGET_H
#define QMLCHATWIDGET_H

#include <QBoxLayout>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>

#include "chatwidgetinterface.h"

#include "chatmessagescontroller.h"
#include "chatuserscontroller.h"

#include "chatmessagesmodel.h"
#include "chatusersmodel.h"

// ==========================================================================================
//                                                                  CHAT WIDGET ON QML ENGINE
// ==========================================================================================
class QmlChatWidget : public ChatWidgetInterface
{
    Q_OBJECT

public:

    QmlChatWidget();
    QmlChatWidget(QmlChatWidget const&)             = delete;
    QmlChatWidget& operator=(QmlChatWidget const &) = delete;

    virtual void keyPressEvent(QKeyEvent *event)                         override;
    virtual void resizeEvent  (QResizeEvent * event)                     override;

public slots:

    virtual void configureOnLogin(const QString &userName)               override;
    virtual void updateTheme     (const QString &themeName)              override;

    virtual void updateOnlineUsers   (const QStringList &onlineUsers)    override;
    virtual void updateConnectedUsers(const QStringList &connectedUsers) override;

    virtual void appendMessage(const QString &messageAuthor,
                               const QString &messageBody)               override;

private:

    // Name of the current user
    QString mUserName;

    // Messages & Users controllers
    ChatMessagesController *mpMessagesController;
    ChatUsersController    *mpUsersController;

    // Qml scene context, current layout and widget
    QQmlContext      *mpCurrentChatContext;
    QBoxLayout       *mpCurrentChatLayout;
    QWidget          *mpCurrentQmlChatWidget;

private slots:

    void shareMessageOnSendingMessage (const ChatMessage &message);

    void ConnectUserOnSwitchOn     (const QString &userName);
    void DisconnectUserOnSwitchOff (const QString &userName);
};

#endif // QMLCHATWIDGET_H
