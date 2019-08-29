#include "qmlchatwidget.h"
// ==========================================================================================
// ==========================================================================================
//                                                                                CONSTRUCTOR
QmlChatWidget::QmlChatWidget()
{
    // Set default user name
    mUserName = QString();

    QQuickView *tmpView = new QQuickView();

    mpCurrentChatContext = tmpView->engine()->rootContext();
    mpCurrentChatContext->setContextProperty("globalTheme", "white");

    tmpView->setResizeMode(QQuickView::SizeRootObjectToView);
    tmpView->setSource(QUrl("qrc:/chatdisabled.qml"));

    mpCurrentQmlChatWidget = QWidget::createWindowContainer(tmpView, this);
    mpCurrentQmlChatWidget->setContentsMargins(0, 0, 0, 0);
    mpCurrentQmlChatWidget->setFocusPolicy(Qt::StrongFocus);
    mpCurrentQmlChatWidget->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);

    mpCurrentChatLayout = new QBoxLayout(QBoxLayout::BottomToTop, this);
    mpCurrentChatLayout->addWidget(mpCurrentQmlChatWidget);
    mpCurrentChatLayout->setSpacing(0);
    mpCurrentChatLayout->setMargin(0);

    setLayout(mpCurrentChatLayout);
    setMinimumSize(210, 400);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                          DOCKER KEY PRESS EVENTS PROCESSOR
void QmlChatWidget::keyPressEvent(QKeyEvent *event)
{
    // idle by now
    event->key();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                             DOCKER RESIZE EVENTS PROCESSOR
void QmlChatWidget::resizeEvent(QResizeEvent *event)
{
    mpCurrentQmlChatWidget->update();
    mpCurrentQmlChatWidget->repaint();
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                          CHAT CONFIGURATOR
void QmlChatWidget::configureOnLogin(const QString &userName)
{
    if (mUserName != QString())
    {
        // Hold the damage & inform user that he/she can't log in second time
        mpMessagesController->sendSystemMessage(SystemMessage::CanNotLogInTwiceMessage);
        return;
    }

    mUserName = userName;

    // Create & connect chat users controller
    mpUsersController = new ChatUsersController(mUserName);
    connect(mpUsersController, &ChatUsersController::userStateChangedConnected,
            this,              &QmlChatWidget::ConnectUserOnSwitchOn,
            Qt::UniqueConnection);
    connect(mpUsersController, &ChatUsersController::userStateChangedDisconnected,
            this,              &QmlChatWidget::DisconnectUserOnSwitchOff,
            Qt::UniqueConnection);
    // Register chat users model & users controller in the QML
    qmlRegisterType<ChatUsersModel>("PairStormChat", 1, 0, "UsersModel");
    qmlRegisterUncreatableType<ChatUsersController>("PairStormChat", 1, 0, "UsersList",
                                                    "Users list can be created only in backend");

    // Create & connect chat messages controller
    mpMessagesController = new ChatMessagesController(userName);
    mpMessagesController->sendSystemMessage(SystemMessage::GreetingsMessage);
    connect(mpMessagesController, &ChatMessagesController::sendingMessage,
            this,                 &QmlChatWidget::shareMessageOnSendingMessage,
            Qt::UniqueConnection);
    // Register chat messages model & messages controller in the QML
    qmlRegisterType<ChatMessagesModel>("PairStormChat", 1, 0, "MessagesModel");
    qmlRegisterUncreatableType<ChatMessagesController>("PairStormChat", 1, 0, "MessagesList",
                                                       "Messages list can be created only in backend");



    QQuickView *tmpView = new QQuickView();

    mpCurrentChatContext = tmpView->engine()->rootContext();
    mpCurrentChatContext->setContextProperty("globalTheme",   "white");
    mpCurrentChatContext->setContextProperty("globalUserName", mUserName);
    mpCurrentChatContext->setContextProperty("messagesList",   mpMessagesController);
    mpCurrentChatContext->setContextProperty("usersList",      mpUsersController);

    tmpView->setResizeMode(QQuickView::SizeRootObjectToView);
    tmpView->setSource(QUrl("qrc:/chat.qml"));

    mpCurrentChatLayout->removeWidget(mpCurrentQmlChatWidget);
    mpCurrentQmlChatWidget->hide();
    delete mpCurrentQmlChatWidget;

    mpCurrentQmlChatWidget = QWidget::createWindowContainer(tmpView, this);
    mpCurrentQmlChatWidget->setContentsMargins(0, 0, 0, 0);
    mpCurrentQmlChatWidget->setFocusPolicy(Qt::StrongFocus);
    mpCurrentQmlChatWidget->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Expanding);

    mpCurrentChatLayout->addWidget(mpCurrentQmlChatWidget);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                          UPDATE ONLINE USERS IN CONTROLLER
void QmlChatWidget::updateOnlineUsers(const QStringList &onlineUsers)
{
    if (mpMessagesController)
    {
        mpUsersController->updateOnlineUsers(onlineUsers);
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                       UPDATE CONNECTED USERS IN CONTROLLER
void QmlChatWidget::updateConnectedUsers(const QStringList &connectedUsers)
{
    if (mpMessagesController)
    {
        mpUsersController->updateConnectedUsers(connectedUsers);
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                             APPEND NEW MESSAGE TO THE CHAT
void QmlChatWidget::appendMessage(const QString &messageAuthor,
                                  const QString &messageBody)
{
    // Here we IGNORE messageAuthor because it is solely NEEDED FOR BACK COMPATIBILITY
    if (!mpMessagesController)
    {
        // Hold the damage if it is called before user has logged in
        return;
    }

    ChatMessage newMessage;
    newMessage.fromJsonQString(messageBody);
    if (newMessage.empty())
    {
        // Hold the damage if json bearer string is broken
        return;
    }

    mpMessagesController->appendMessage(newMessage);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                                                          UPDATE CHAT THEME
void QmlChatWidget::updateTheme(const QString &themeName)
{
    Theme newTheme = mThemes[themeName];
    switch(newTheme)
    {
    case Theme::DefaultTheme:
        mpCurrentChatContext->setContextProperty("globalTheme", "white");
        break;
    case Theme::WhiteTheme:
        mpCurrentChatContext->setContextProperty("globalTheme", "white");
        break;
    case Theme::BlueTheme:
        mpCurrentChatContext->setContextProperty("globalTheme", "blue");
        break;
    case Theme::DarkTheme:
        mpCurrentChatContext->setContextProperty("globalTheme", "dark");
        break;
    }
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                        SHARE MESSAGE WHEN USER INVOKES SEND MESSAGE ACTION
void QmlChatWidget::shareMessageOnSendingMessage(const ChatMessage &message)
{
    const QString messageString = message.toJsonQString();
    emit messageSent(messageString);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                         SEND CONNECT REQUEST WHEN USER TRUNS ON THE SWITCH
void QmlChatWidget::ConnectUserOnSwitchOn(const QString &userName)
{
    emit startSharingRequested(userName);
}
// ==========================================================================================
// ==========================================================================================
// ==========================================================================================
//                                     SEND DISCONNECT REQUEST WHEN USER TRUNS OFF THE SWITCH
void QmlChatWidget::DisconnectUserOnSwitchOff(const QString &userName)
{
    emit stopSharingRequested(userName);
}
