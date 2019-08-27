#include "chatwidget.h"
// ==========================================================================================
// ==========================================================================================
//                                                                                CONSTRUCTOR
ChatWidget::ChatWidget()
{
    // List of online users
    mpUsersList = new QListWidget;
    mpUsersList->setSizePolicy(QSizePolicy::Maximum,
                               QSizePolicy::Maximum);
    connect(mpUsersList, &QListWidget::itemDoubleClicked,
            this,        &ChatWidget::shareWithUserOnDoubleClick,
            Qt::AutoConnection);

    // List of messages & input message area
    mpMessagesHistory   = new QPlainTextEdit;
    mpInputMessageField = new QLineEdit;

    // Send button
    QPushButton *pSendButton = new QPushButton("Send");
    pSendButton->setMaximumWidth(30);
    connect(pSendButton, &QPushButton::clicked,
            this,        &ChatWidget::shareMessageOnSend,
            Qt::UniqueConnection);

    // Message input area' layout
    QHBoxLayout *pLineLayout = new QHBoxLayout;
    pLineLayout->addWidget(mpInputMessageField);
    pLineLayout->addWidget(pSendButton);

    // General layout
    QVBoxLayout *pWindowLayout = new QVBoxLayout;
    pWindowLayout->addWidget(mpUsersList);
    pWindowLayout->addWidget(mpMessagesHistory);
    pWindowLayout->addLayout(pLineLayout);


    setLayout(pWindowLayout);
    setMinimumSize(150, 300);
    setDisabled(true);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                          DOCKER KEY PRESS EVENTS PROCESSOR
void ChatWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter
       || event->key() == Qt::Key_Return)
    {
        shareMessageOnSend();
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                          CHAT CONFIGURATOR
void ChatWidget::configureOnLogin(const QString &userName)
{
    mUserName = userName;
    setDisabled(false);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                 ONLINE USERS LIST RENDERER
void ChatWidget::redrawUsersList()
{
    mpUsersList->clear();

    QListWidgetItem *pUserItem;
    for (const QString &userName: mOnlineUsers)
    {
        pUserItem = new QListWidgetItem;
        pUserItem->setText("Connect to: " + userName);

        pUserItem->setIcon(mConnectedUsers.contains(userName) ?
                               QIcon(":/img/CONNECTED.png")   :
                               QIcon(":/img/DISCONNECTED.png"));

        mpUsersList->addItem(pUserItem);
    }

    mpUsersList->setSizePolicy(QSizePolicy::Maximum,
                               QSizePolicy::Maximum);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                               UPDATE ONLINE USERS & REDRAW
void ChatWidget::updateOnlineUsers(const QStringList &onlineUsers)
{
    mOnlineUsers = onlineUsers;
    redrawUsersList();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                            UPDATE CONNECTED USERS & REDRAW
void ChatWidget::updateConnectedUsers(const QStringList &connectedUsers)
{
    mConnectedUsers = connectedUsers;
    redrawUsersList();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                             APPEND NEW MESSAGE TO THE CHAT
void ChatWidget::appendMessage(const QString &messageAuthor,
                               const QString &messageBody)
{
    QString chatMessage = mpMessagesHistory->toPlainText();
    chatMessage += '\n';
    QString content = "[ " + messageAuthor + " ]  " + messageBody;
    chatMessage += content;
    mpMessagesHistory->setPlainText(chatMessage);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                          UPDATE CHAT THEME
void ChatWidget::updateTheme(const QString &themeName)
{
    // TODO
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                        SHARE MESSAGE WHEN USER INVOKES SEND MESSAGE ACTION
void ChatWidget::shareMessageOnSend()
{
    // Render message in the chat
    QString chatMessage = mpMessagesHistory->toPlainText();
    chatMessage += '\n';
    QString content = "[ " + mUserName + " ]  " + mpInputMessageField->text();
    chatMessage += content;
    mpMessagesHistory->setPlainText(chatMessage);

    // Share message with collegues
    emit messageSent(mpInputMessageField->text());

    // Clear message input field
    mpInputMessageField->clear();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                     SEND CONNECT OR DISCONNECT REQUEST WHEN USER DOUBLECLICKS THE NEIGHBOR
void ChatWidget::shareWithUserOnDoubleClick(QListWidgetItem *userItem)
{
    QString userName = userItem->text();
    int position = userName.lastIndexOf(QChar{':'});
    QString bareName = userName.mid(position + 2);

    if (mConnectedUsers.contains(bareName))
    {
        emit stopSharingRequested(bareName);
    }
    else
    {
        emit startSharingRequested(bareName);
    }
}
