#include "qmlchatwidget.h"

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

QmlChatWidget::QmlChatWidget()
{
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
    mpUsers = new OnlineUsersModel();

    connect(mpUsers, &OnlineUsersModel::stateChangedOn,
            this,          &QmlChatWidget::ConnectUserOnChangedState,
            Qt::UniqueConnection);

    connect(mpUsers, &OnlineUsersModel::stateChangedOff,
            this,          &QmlChatWidget::DisconnectUserOnChangedState,
            Qt::UniqueConnection);
}

void QmlChatWidget::keyPressEvent(QKeyEvent *event)
{
    event->key();
    //pass
}

void QmlChatWidget::configureOnLogin(const QString &userName)
{
    mUserName = userName;
    qmlRegisterType<OnlineUsersModel>("AvailableUsers", 1, 0, "AvailableUsersModel");


    QBoxLayout *box = new QBoxLayout(QBoxLayout::BottomToTop, this);
    box->setSpacing(0);
    box->setMargin(0);

    QQuickView * view = new QQuickView();

    QSurfaceFormat format;
    format.setSamples(16);
    view->setFormat(format);

    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/chat.qml"));
    mpChatContext = view->engine()->rootContext();
    mpChatContext->setContextProperty(QStringLiteral("AvailableUsersList"), mpUsers);

    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setContentsMargins(0, 0, 0, 0);

    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    //setEnabled(false);
    setMinimumSize(210, 400);

    box->addWidget(container);
    setLayout(box);
    show();
}

void QmlChatWidget::updateOnlineUsers(const QStringList &onlineUsers)
{
    mpUsers->updateOnlineUsers(onlineUsers);
}

void QmlChatWidget::updateConnectedUsers(const QStringList &connectedUsers)
{
    mpUsers->updateConnectedUsers(connectedUsers);
}

void QmlChatWidget::appendMessage(const QString &messageAuthor,
                                  const QString &messageBody)
{
    qDebug() << "append message: " << messageBody;
    qDebug() << "author:         " << messageAuthor;
}

void QmlChatWidget::ConnectUserOnChangedState(const QString userName)
{
    emit startSharingRequested(userName);
}

void QmlChatWidget::DisconnectUserOnChangedState(const QString userName)
{
    emit stopSharingRequested(userName);
}
