#ifndef NEWUSERWINDOW_H
#define NEWUSERWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QLineEdit;
//class QListWidget;
class QAbstractButton;
//class QListWidgetItem;
QT_END_NAMESPACE


class NewUserWindow : public QDialog
{
    Q_OBJECT
public:
    explicit NewUserWindow(QWidget *parent = nullptr);
private:
    //QLabel *mpLabelTitle;
    QLabel *mpLabelLogin;
    QLabel *mpLabelToken;
    QLineEdit *mpEditLogin;
    QLineEdit *mpEditToken;

    QPushButton *mpUnnamedUser;

    QDialogButtonBox *mpButtonBox;

    // design parameters
    //int mColumnSpasing              = 10;
    //int mStretchFactor              = 20;
    //int mStretchFactorMain          = 100;
    int mLabelWidth                 = 130;
    int mEditWidth                  = 320;
    int mBasicStretch               = 1;
    int mSeparatorWidth             = 4;
    int mUnnamedUserButtonWidth     = 200;

public slots:
    void onBtnBoxClicked(QAbstractButton * button);

signals:
    void cancel();
    void unnamedUser();
    void newUserToken(const QString &login, const QString &token);
    void newUserPasssword(const QString &login, const QString &passwrd);
};

#endif // NEWUSERWINDOW_H
