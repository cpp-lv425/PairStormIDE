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
public:
    ChatWidget(QWidget *pParent = nullptr);
    void setUsersList(const QStringList& usersList);

private slots:
    void onUserToConnectSelected(QListWidgetItem* item);

signals:
    void userToConnectSelected(QString);
};

#endif // CHATWIDGET_H
