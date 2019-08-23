#ifndef QMLCHATWIDGET_H
#define QMLCHATWIDGET_H

#include "chatwidgetinterface.h"
//#include "onlineusersmodel.h"

#include "chatmessagesmodel.h"
#include "chatmessagescontroller.h"

#include "chatusersmodel.h"
#include "chatuserscontroller.h"

#include <QQmlContext>
#include <QBoxLayout>

class QmlChatWidget : public ChatWidgetInterface
{
    Q_OBJECT

public:
    QmlChatWidget();
    QmlChatWidget(QmlChatWidget const&)             = delete;
    QmlChatWidget& operator=(QmlChatWidget const &) = delete;

    virtual void keyPressEvent(QKeyEvent * event) override;

public slots:

    virtual void configureOnLogin(const QString & userName) override;

    virtual void updateOnlineUsers(const QStringList & onlineUsers) override;
    virtual void updateConnectedUsers(const QStringList & connectedUsers) override;

    virtual void appendMessage(const QString & messageAuthor,
                               const QString & messageBody) override;

    virtual void updateTheme(const QString & themeName) override;

private:

    QString mUserName;

    ChatMessagesController * mpMessagesController;
    ChatUsersController    * mpUsersController;

    QQmlContext      * mpCurrentChatContext;
    QBoxLayout       * mpCurrentChatLayout;
    QWidget          * mpCurrentQmlChatWidget;

private slots:

    void shareMessageOnSendingMessage (const ChatMessage & message);
    void ConnectUserOnChangedState    (const QString userName);
    void DisconnectUserOnChangedState (const QString userName);

    void updateToFitCurrentTheme();

};

#endif // QMLCHATWIDGET_H
