#include "qmlchatwidget.h"

#include <QPlainTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QBoxLayout>
#include <QQuickView>
#include <QQuickWidget>
#include <QLineEdit>
#include <QLabel>

#include <QGuiApplication>

#include <QHBoxLayout>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>
#include <QDockWidget>
#include <QQuickStyle>
#include <QDebug>

QmlChatWidget::QmlChatWidget() : mpMessagesController(nullptr)
{
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
    /*
    mpUsers = new OnlineUsersModel();

    connect(mpUsers, &OnlineUsersModel::stateChangedOn,
            this,          &QmlChatWidget::ConnectUserOnChangedState,
            Qt::UniqueConnection);

    connect(mpUsers, &OnlineUsersModel::stateChangedOff,
            this,          &QmlChatWidget::DisconnectUserOnChangedState,
            Qt::UniqueConnection);

*/




    mpBoxLayout = new QBoxLayout(QBoxLayout::BottomToTop, this);
    mpBoxLayout->setSpacing(0);
    mpBoxLayout->setMargin(0);

    QQuickView * view = new QQuickView();

    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/chatdisabled.qml"));

    mpRestrictedChatWidget = QWidget::createWindowContainer(view, this);
    mpRestrictedChatWidget->setContentsMargins(0, 0, 0, 0);

    mpRestrictedChatWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mpRestrictedChatWidget->setFocusPolicy(Qt::StrongFocus);

    //setEnabled(false);
    setMinimumSize(210, 400);

    mpBoxLayout->addWidget(mpRestrictedChatWidget);
    setLayout(mpBoxLayout);
}

void QmlChatWidget::keyPressEvent(QKeyEvent *event)
{
    event->key();
    //pass
}

void QmlChatWidget::configureOnLogin(const QString &userName)
{
    if (mpMessagesController)
    {
        mpMessagesController->sendCanNotLogInTwiceMessage();
        //mpUsersController->updateOnlineUsers(QStringList() << "lilia" << "petro");
        //updateConnectedUsers(QStringList() << "bodia");
        //updateOnlineUsers(QStringList() << "laila" << "dima");

        return;
    }

    mUserName = userName;

    mpMessagesController = new ChatMessagesController(userName);
    mpMessagesController->sendGreetingsMessage();

    connect(mpMessagesController, &ChatMessagesController::sendingMessage,
            this,                 &QmlChatWidget::shareMessageOnSendingMessage,
            Qt::UniqueConnection);

    mpUsersController = new ChatUsersController();

    connect(mpUsersController, &ChatUsersController::userStateChangedConnected,
            this,              &QmlChatWidget::ConnectUserOnChangedState,
            Qt::UniqueConnection);

    connect(mpUsersController, &ChatUsersController::userStateChangedDisconnected,
            this,              &QmlChatWidget::DisconnectUserOnChangedState,
            Qt::UniqueConnection);

    qmlRegisterType<ChatUsersModel>("PairStormChat", 1, 0, "UsersModel");
    qmlRegisterUncreatableType<ChatUsersController>("PairStormChat", 1, 0, "UsersList",
                                                       QStringLiteral("Users list can be created only in backend"));


    qmlRegisterType<ChatMessagesModel>("PairStormChat", 1, 0, "MessagesModel");
    qmlRegisterUncreatableType<ChatMessagesController>("PairStormChat", 1, 0, "MessagesList",
                                                       QStringLiteral("Messages list can be created only in backend"));






    QQuickView * view = new QQuickView();

    QSurfaceFormat format;
    format.setSamples(16);
    view->setFormat(format);

    view->setResizeMode(QQuickView::SizeRootObjectToView);

    mpChatContext = view->engine()->rootContext();
    //mpChatContext->setContextProperty(QStringLiteral("AvailableUsersList"), mpUsers);
    //mpChatContext->setContextProperty(QStringLiteral("usersList"), mpUsers->getUsersList());

    //============================================================================================
    mpChatContext->setContextProperty(QStringLiteral("globalUserName"), mUserName);

    mpChatContext->setContextProperty(QStringLiteral("messagesList"),   mpMessagesController);
    mpChatContext->setContextProperty(QStringLiteral("usersList"),      mpUsersController);
    //============================================================================================

    view->setSource(QUrl("qrc:/chat.qml"));



    mpAllowedChatWidget = QWidget::createWindowContainer(view, this);
    mpAllowedChatWidget->setContentsMargins(0, 0, 0, 0);

    mpAllowedChatWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mpAllowedChatWidget->setFocusPolicy(Qt::StrongFocus);

    mpBoxLayout->removeWidget(mpRestrictedChatWidget);
    mpRestrictedChatWidget->hide();
    delete mpRestrictedChatWidget;
    mpBoxLayout->addWidget(mpAllowedChatWidget);

    //updateOnlineUsers(QStringList() << "bodia");
}

void QmlChatWidget::updateOnlineUsers(const QStringList &onlineUsers)
{
    //mpUsers->updateOnlineUsers(onlineUsers);
    mpUsersController->updateOnlineUsers(onlineUsers);
}

void QmlChatWidget::updateConnectedUsers(const QStringList &connectedUsers)
{
    //mpUsers->updateConnectedUsers(connectedUsers);
    mpUsersController->updateConnectedUsers(connectedUsers);
}

void QmlChatWidget::appendMessage(const QString &messageAuthor,
                                  const QString &messageBody)
{
    if(!mpMessagesController)
    {
        return;
    }
    // Here we ignore messageAuthor because it is solely needed for back compatibility
    ChatMessage newMessage;
    newMessage.fromJsonQString(messageBody);
    if (newMessage.empty())
    {
        return;
    }

    qDebug() << "append message: " << messageBody;
    qDebug() << "author:         " << messageAuthor;

    mpMessagesController->appendMessage(newMessage);
}

void QmlChatWidget::shareMessageOnSendingMessage(const ChatMessage &message)
{
    const QString messageString = message.toJsonQString();
    emit messageSent(messageString);
}

void QmlChatWidget::ConnectUserOnChangedState(const QString userName)
{
    emit startSharingRequested(userName);
}

void QmlChatWidget::DisconnectUserOnChangedState(const QString userName)
{
    emit stopSharingRequested(userName);
}
