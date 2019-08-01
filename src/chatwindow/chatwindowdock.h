#ifndef CHATWINDOWDOCK_H
#define CHATWINDOWDOCK_H

#include <QDockWidget>

class ChatWidget;

class ChatWindowDock: public QDockWidget
{
    Q_OBJECT

    ChatWidget *mpChatWidget;
public:
    explicit ChatWindowDock(QWidget *pParent = nullptr);
    void setUserName(const QString &userName);
    void displayMessage(const QString &userName,
                        const QString &message);

signals:
    void userToConnectSelected(QString);
    void sendMessage(const QString&, const QString&);

public slots:
    void onUserToConnectSelected(QString userName);
    void onSendMessage(const QString &userName,
                       const QString &message);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CHATWINDOWDOCK_H
