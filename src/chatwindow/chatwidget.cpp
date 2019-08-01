#include "chatwidget.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QDebug> // temp
#include <QLabel>

ChatWidget::ChatWidget(QWidget *pParent):
    QWidget (pParent),
    mUserName("Unnamed")
{    
    // creating user list
    mpUsersList = new QListWidget;
    connect(mpUsersList, &QListWidget::itemDoubleClicked,
            this, &ChatWidget::onUserToConnectSelected);

    mpUsersList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // creating chat feed
    mpFeed = new QPlainTextEdit;

    // creating enter line
    mpEnterLine = new QLineEdit;

    // creating send button
    QPushButton *pSendButton = new QPushButton("Send");
    pSendButton->setMaximumWidth(30);
    connect(pSendButton, &QPushButton::clicked,
            this, &ChatWidget::onSendCommand);

    QHBoxLayout *pLineLayout = new QHBoxLayout;
    pLineLayout->addWidget(mpEnterLine);
    pLineLayout->addWidget(pSendButton);

    // laying out window
    QVBoxLayout *pWindowLayout = new QVBoxLayout;
    pWindowLayout->addWidget(mpUsersList);

    pWindowLayout->addWidget(mpFeed);
    pWindowLayout->addLayout(pLineLayout);
    setLayout(pWindowLayout);

    // temp
    QStringList users;
    users << "London" << "Berlin" << "Paris";
    setUsersList(users);
}

void ChatWidget::setUsersList(const QStringList &usersList)
{
    QListWidgetItem *pItem;
    for (const QString& userName: usersList)
    {
        pItem = new QListWidgetItem;
        pItem->setText("Connect to: " + userName);
        pItem->setIcon(style()->standardIcon(QStyle::SP_DialogNoButton));
        mpUsersList->addItem(pItem);
    }
    mpUsersList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void ChatWidget::setCurrentUserName(const QString &userName)
{
    mUserName = userName;
}

void ChatWidget::displayMessage(const QString &userName,
                                const QString &message)
{
    QString feed = mpFeed->toPlainText();
    feed += '\n';
    QString insertion = "<" + userName + "> " + message;
    feed += insertion;
    mpFeed->setPlainText(feed);
}

void ChatWidget::onUserToConnectSelected(QListWidgetItem *item)
{
    qDebug() << "chat slot";
    QString userName = item->text();
    int position = userName.lastIndexOf(QChar{':'});
    emit userToConnectSelected(userName.mid(position + 2));
}

void ChatWidget::onSendCommand()
{
    updateFeedOnSend();
}

void ChatWidget::updateFeedOnSend()
{
    QString feed = mpFeed->toPlainText();
    feed += '\n';
    QString insertion = "<" + mUserName + "> " + mpEnterLine->text();
    feed += insertion;
    mpFeed->setPlainText(feed);
    mpEnterLine->clear();
}
