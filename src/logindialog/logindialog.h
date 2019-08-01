#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;

class LoginDialog: public QDialog
{
    QLineEdit *mpUserNameLineEdit;
    QString mUserName;
public:    
    LoginDialog(QWidget *pParent = nullptr);
    QString start();

private slots:
    void onOkButtonClicked();
};

#endif // LOGINDIALOG_H
