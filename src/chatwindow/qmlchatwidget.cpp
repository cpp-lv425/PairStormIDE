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

QmlChatWidget::QmlChatWidget()
{
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
    mUserName = QString();
    updateToFitCurrentTheme();




    QQuickView * view = new QQuickView();

    mpCurrentChatContext = view->engine()->rootContext();

    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/chatdisabled.qml"));

    mpCurrentQmlChatWidget = QWidget::createWindowContainer(view, this);
    mpCurrentQmlChatWidget->setContentsMargins(0, 0, 0, 0);

    mpCurrentQmlChatWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mpCurrentQmlChatWidget->setFocusPolicy(Qt::StrongFocus);

    mpCurrentChatLayout = new QBoxLayout(QBoxLayout::BottomToTop, this);
    mpCurrentChatLayout->setSpacing(0);
    mpCurrentChatLayout->setMargin(0);

    mpCurrentChatLayout->addWidget(mpCurrentQmlChatWidget);
    setLayout(mpCurrentChatLayout);

    setMinimumSize(210, 400);
}

void QmlChatWidget::keyPressEvent(QKeyEvent *event)
{
    event->key();
    //pass
}

void QmlChatWidget::configureOnLogin(const QString &userName)
{
    if (mUserName != QString())
    {
        mpMessagesController->sendCanNotLogInTwiceMessage();
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

    mpCurrentChatContext = view->engine()->rootContext();
    //mpChatContext->setContextProperty(QStringLiteral("AvailableUsersList"), mpUsers);
    //mpChatContext->setContextProperty(QStringLiteral("usersList"), mpUsers->getUsersList());

    //============================================================================================
    mpCurrentChatContext->setContextProperty(QStringLiteral("globalUserName"), mUserName);

    mpCurrentChatContext->setContextProperty(QStringLiteral("messagesList"),   mpMessagesController);
    mpCurrentChatContext->setContextProperty(QStringLiteral("usersList"),      mpUsersController);
    //============================================================================================

    view->setSource(QUrl("qrc:/chat.qml"));

    mpCurrentChatLayout->removeWidget(mpCurrentQmlChatWidget);

    mpCurrentQmlChatWidget->hide();
    delete mpCurrentQmlChatWidget;

    mpCurrentQmlChatWidget = QWidget::createWindowContainer(view, this);

    mpCurrentQmlChatWidget->setContentsMargins(0, 0, 0, 0);
    mpCurrentQmlChatWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mpCurrentQmlChatWidget->setFocusPolicy(Qt::StrongFocus);

    mpCurrentChatLayout->addWidget(mpCurrentQmlChatWidget);
}

void QmlChatWidget::updateOnlineUsers(const QStringList &onlineUsers)
{
    if(!mpMessagesController)
    {
        return;
    }
    mpUsersController->updateOnlineUsers(onlineUsers);
}

void QmlChatWidget::updateConnectedUsers(const QStringList &connectedUsers)
{
    if(!mpMessagesController)
    {
        return;
    }
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

    mpMessagesController->appendMessage(newMessage);
}

void QmlChatWidget::updateTheme(const QString &themeName)
{
    //This can be called even if user is not logged in
    mCurrentTheme = mThemes[themeName];
    updateToFitCurrentTheme();
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

void QmlChatWidget::updateToFitCurrentTheme()
{
    switch(mCurrentTheme)
    {
    case Theme::DefaultTheme:
        mpCurrentChatContext->setContextProperty(QStringLiteral("globalTheme"), "white");
        qDebug() << "CHAT: theme set to white (default)";
        break;
    case Theme::WhiteTheme:
        mpCurrentChatContext->setContextProperty(QStringLiteral("globalTheme"), "white");
        qDebug() << "CHAT: theme set to white";
        break;
    case Theme::BlueTheme:
        mpCurrentChatContext->setContextProperty(QStringLiteral("globalTheme"), "blue");
        qDebug() << "CHAT: theme set to blue";
        break;
    case Theme::DarkTheme:
        mpCurrentChatContext->setContextProperty(QStringLiteral("globalTheme"), "dark");
        qDebug() << "CHAT: theme set to dark";
        break;
    }
}
