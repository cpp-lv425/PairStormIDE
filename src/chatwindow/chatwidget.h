#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

class QListWidgetItem;
class QPlainTextEdit;
class QListWidget;
class QLineEdit;

class ChatWidget: public QWidget
{
    Q_OBJECT

    QListWidget *mpUsersList;
    QStringList  mOnlineUsers;
    QStringList  mConnectedUsers;
    QString      mUserName;

    QPlainTextEdit *mpFeed;
    QLineEdit      *mpEnterLine;

    void updateUsersList();

public:
    explicit ChatWidget(QWidget *pParent = nullptr);
    void setOnlineUsers(const QStringList & onlineUsers);
    void setConnectedUsers(const QStringList & connectedUsers);
    void setCurrentUserName(const QString& userName);
    void displayMessage(const QString& userName,
                        const QString& message);

public slots:
    void onSendCommand();

private slots:
    void onUserToConnectSelected(QListWidgetItem *item);
    void updateFeedOnSend();

signals:
    void userToConnectSelected(QString);
    void sendMessage(const QString&);
};

#endif // CHATWIDGET_H
