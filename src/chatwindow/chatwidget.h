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
    QPlainTextEdit *mpFeed;
    QLineEdit *mpEnterLine;
    QString mUserName;
public:
    ChatWidget(QWidget *pParent = nullptr);
    void setUsersList(const QStringList& usersList);
    void setCurrentUserName(const QString& userName);
    void displayMessage(const QString& userName,
                        const QString& message);

public slots:
    void onSendCommand();

private slots:
    void onUserToConnectSelected(QListWidgetItem* item);    
    void updateFeedOnSend();

signals:
    void userToConnectSelected(QString);
    void sendMessage(const QString&);
};

#endif // CHATWIDGET_H
