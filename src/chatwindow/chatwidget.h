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

private slots:
    void onUserToConnectSelected(QListWidgetItem* item);
    void onSendCommand();

signals:
    void userToConnectSelected(QString);
};

#endif // CHATWIDGET_H
