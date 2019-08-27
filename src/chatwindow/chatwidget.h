#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QLineEdit>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QListWidgetItem>

#include "chatwidgetinterface.h"

// ==========================================================================================
//                                                                   CHAT WIDGET ON QT ENGINE
// ==========================================================================================
class ChatWidget: public ChatWidgetInterface
{
    Q_OBJECT

public:

    ChatWidget();
    ChatWidget(ChatWidget const&)             = delete;
    ChatWidget& operator=(ChatWidget const&)  = delete;

    virtual void keyPressEvent(QKeyEvent *event)                         override;

public slots:

    virtual void configureOnLogin(const QString &userName)               override;
    virtual void updateTheme     (const QString &themeName)              override;

    virtual void updateOnlineUsers   (const QStringList &onlineUsers)    override;
    virtual void updateConnectedUsers(const QStringList &connectedUsers) override;

    virtual void appendMessage(const QString &messageAuthor,
                               const QString &messageBody)               override;

private:

    // Name of the current user
    QString      mUserName;

    // Lists of online as well as connected users
    QStringList  mOnlineUsers;
    QStringList  mConnectedUsers;

    // Users list, editor & message input fields
    QListWidget    *mpUsersList;
    QPlainTextEdit *mpMessagesHistory;
    QLineEdit      *mpInputMessageField;

    void redrawUsersList();

private slots:

    void shareWithUserOnDoubleClick(QListWidgetItem *userItem);
    void shareMessageOnSend();
};

#endif // CHATWIDGET_H
