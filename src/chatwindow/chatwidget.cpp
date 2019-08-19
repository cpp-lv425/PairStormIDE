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

#include "onlineuserslist.h"
#include "availableusersmodel.h"

ChatWidget::ChatWidget(QWidget *pParent):
    QWidget (pParent),
    mUserName("Unnamed")
{
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
    qmlRegisterType<AvailableUsersModel>("AvailableUsers", 1, 0, "AvailableUsersModel");

    mpOnlineUsers = new OnlineUsersList();

    QBoxLayout *box = new QBoxLayout(QBoxLayout::BottomToTop, this);
    box->setSpacing(0);
    box->setMargin(0);


    QQuickView * view = new QQuickView();
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/chat.qml"));
    mpChatContext = view->engine()->rootContext();
    mpChatContext->setContextProperty(QStringLiteral("AvailableUsersList"), mpOnlineUsers);

    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setContextMenuPolicy(Qt::NoContextMenu);
    container->setContentsMargins(0, 0, 0, 0);

    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);


    setWindowIcon(QIcon(":/chatelements/res/CONNECTED.png"));
    setWindowTitle(QString("QML chat"));
    setEnabled(false);
    setMinimumSize(210, 400);

    box->addWidget(container);
    setLayout(box);

    hide();

    /*
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

    // Set empty users list
    updateUsersList();
    */
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

void ChatWidget::setOnlineUsers(const QStringList &onlineUsers)
{
    mOnlineUsers = onlineUsers;
    updateUsersList();
}

void ChatWidget::setConnectedUsers(const QStringList &connectedUsers)
{
    mConnectedUsers = connectedUsers;
    updateUsersList();
}

bool ChatWidget::isUserConnected(const QString &userName)
{
    return mConnectedUsers.contains(userName);
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
    QString userName = item->text();
    int position = userName.lastIndexOf(QChar{':'});
    emit userToConnectSelected(userName.mid(position + 2));
}

void ChatWidget::onSendCommand()
{
    emit sendMessage(mpEnterLine->text());
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
