#include "chatwidget.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QBoxLayout>
#include <QQuickView>
#include <QQuickWidget>
#include <QLineEdit>
#include <QLabel>

#include <QHBoxLayout>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <QDockWidget>
#include <QQuickStyle>

//#include "onlineuserslist.h"
//#include "onlineusersmodel.h"

ChatWidget::ChatWidget()
{
    // creating user list
    mpUsersList = new QListWidget;
    connect(mpUsersList, &QListWidget::itemDoubleClicked,
            this,        &ChatWidget::connectOrDisconnectOnRequest,
            Qt::AutoConnection);

    mpUsersList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // creating chat feed
    mpFeed = new QPlainTextEdit;

    // creating enter line
    mpEnterLine = new QLineEdit;

    // creating send button
    QPushButton *pSendButton = new QPushButton("Send");

    pSendButton->setMaximumWidth(30);
    connect(pSendButton, &QPushButton::clicked,
            this,        &ChatWidget::onSendCommand);

    QHBoxLayout *pLineLayout = new QHBoxLayout;
    pLineLayout->addWidget(mpEnterLine);
    pLineLayout->addWidget(pSendButton);

    // laying out window
    QVBoxLayout *pWindowLayout = new QVBoxLayout;
    pWindowLayout->addWidget(mpUsersList);

    pWindowLayout->addWidget(mpFeed);
    pWindowLayout->addLayout(pLineLayout);

    setLayout(pWindowLayout);

    // Set empty users list
    updateUsersList();
}

void ChatWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter
       || event->key() == Qt::Key_Return)
    {
        //ChatWidgetInterface::keyPressEvent(event);
        onSendCommand();
        qDebug() << "send message pressed";
    }
}

void ChatWidget::configureOnLogin(const QString & userName)
{
    mUserName = userName;
}

void ChatWidget::updateUsersList()
{
    QListWidgetItem *pItem;
    mpUsersList->clear();
    for (const QString& userName: mOnlineUsers)
    {
        pItem = new QListWidgetItem;
        pItem->setText("Connect to: " + userName);
        if (mConnectedUsers.contains(userName))
        {
            pItem->setIcon(QIcon(":/img/CONNECTED.png"));
        }
        else
        {
            pItem->setIcon(QIcon(":/img/DISCONNECTED.png"));
        }
        mpUsersList->addItem(pItem);
    }
    mpUsersList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void ChatWidget::updateOnlineUsers(const QStringList & onlineUsers)
{
    mOnlineUsers = onlineUsers;
    updateUsersList();
}

void ChatWidget::updateConnectedUsers(const QStringList & connectedUsers)
{
    mConnectedUsers = connectedUsers;
    updateUsersList();
}

void ChatWidget::appendMessage(const QString & messageAuthor,
                               const QString & messageBody)
{
    QString feed = mpFeed->toPlainText();
    feed += '\n';
    QString insertion = "<" + messageAuthor + "> " + messageBody;
    feed += insertion;
    mpFeed->setPlainText(feed);
}

void ChatWidget::connectOrDisconnectOnRequest(QListWidgetItem *item)
{
    QString userName = item->text();
    int position = userName.lastIndexOf(QChar{':'});
    QString bareName = userName.mid(position + 2);

    if (mConnectedUsers.contains(bareName))
    {
        emit startSharingRequested(bareName);
    }
    else
    {
        emit stopSharingRequested(bareName);
    }
}

void ChatWidget::onSendCommand()
{
    emit messageSent(mpEnterLine->text());
    QString feed = mpFeed->toPlainText();
    feed += '\n';
    QString insertion = "<" + mUserName + "> " + mpEnterLine->text();
    feed += insertion;
    mpFeed->setPlainText(feed);
    mpEnterLine->clear();
}
