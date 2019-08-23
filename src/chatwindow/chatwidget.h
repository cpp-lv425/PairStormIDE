#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include "chatwidgetinterface.h"

class OnlineUsersList;
class QListWidgetItem;
class QPlainTextEdit;
class QQuickWidget;
class QQmlContext;
class QListWidget;
class QLineEdit;


class ChatWidget: public ChatWidgetInterface
{
    Q_OBJECT

public:
    ChatWidget();
    ChatWidget(ChatWidget const&)             = delete;
    ChatWidget & operator=(ChatWidget const&) = delete;

    virtual void keyPressEvent(QKeyEvent * event) override;

public slots:

    virtual void configureOnLogin(const QString & userName) override;

    virtual void updateOnlineUsers(const QStringList & onlineUsers) override;
    virtual void updateConnectedUsers(const QStringList & connectedUsers) override;

    virtual void appendMessage(const QString & messageAuthor,
                               const QString & messageBody) override;

    virtual void updateTheme(const QString & themeName) override;

private slots:

    void connectOrDisconnectOnRequest(QListWidgetItem *item);
    void onSendCommand();

private:
    QListWidget *mpUsersList;
    QStringList  mOnlineUsers;
    QStringList  mConnectedUsers;
    QString      mUserName;

    QPlainTextEdit *mpFeed;
    QLineEdit      *mpEnterLine;

    void updateUsersList();
};

#endif // CHATWIDGET_H
