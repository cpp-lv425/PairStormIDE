#include "chatwidget.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QLabel>

ChatWidget::ChatWidget(QWidget *pParent): QWidget (pParent)
{    
    // creating user list
    mpUsersList = new QListWidget;

    mpUsersList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // creating chat feed
    mpFeed = new QPlainTextEdit;

    // creating enter line
    mpEnterLine = new QLineEdit;

    // creating send button
    QPushButton *pSendButton = new QPushButton("Send");
    QHBoxLayout *pLineLayout = new QHBoxLayout;
    pLineLayout->addWidget(mpEnterLine);
    pLineLayout->addWidget(pSendButton);

    // laying out window
    QVBoxLayout *pWindowLayout = new QVBoxLayout;
    pWindowLayout->addWidget(mpUsersList);

    pWindowLayout->addWidget(mpFeed);
    pWindowLayout->addLayout(pLineLayout);
    setLayout(pWindowLayout);

    // temp solution
    QStringList usersList;
    usersList << "Valik" << "Ihog" << "Alex" << "Nastia";
    setUsersList(usersList);
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
