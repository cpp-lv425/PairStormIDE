#ifndef CHATWINDOWDOCK_H
#define CHATWINDOWDOCK_H

#include <QDockWidget>

class ChatWidget;

class ChatWindowDock: public QDockWidget
{
    Q_OBJECT

    ChatWidget *mpChatWidget;
public:
    ChatWindowDock(QWidget *pParent = nullptr);
    void setUserName(const QString& userName);
    void displayMessage(const QString& userName,
                        const QString& message);

signals:
    void userToConnectSelected(QString);

public slots:
    void onUserToConnectSelected(QString userName);

};

#endif // CHATWINDOWDOCK_H
