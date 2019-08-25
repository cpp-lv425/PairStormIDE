#ifndef NEWUSERWINDOW_H
#define NEWUSERWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QAbstractButton;
class QLineEdit;
class QString;
class QLabel;
QT_END_NAMESPACE

class NewUserWindow : public QDialog
{
    Q_OBJECT
public:
    explicit NewUserWindow(QWidget *parent = nullptr);
private:
    QLabel *mpLabelLogin;
    QLabel *mpLabelToken;
    QLineEdit *mpEditLogin;
    QLineEdit *mpEditToken;
    QString mPlaceholderText {"Sequence with 40 characters treats as token"};

    QPushButton *mpUnnamedUserButton;

    QDialogButtonBox *mpButtonBox;

    // design parameters
    int mLabelWidth                 = 130;
    int mEditWidth                  = 320;
    int mBasicStretch               = 1;
    int mSeparatorWidth             = 4;
    int mUnnamedUserButtonWidth     = 200;

public slots:
    void onBtnBoxClicked(QAbstractButton * button);

signals:
    void cancel();                              // user closed window - termination application
    void unnamedUser();                                             //  user chose nunamed mode
    void newUserToken(const QString &login, const QString &token);  //  user typed login and token to registrate
    void newUserPasssword(const QString &login, const QString &passwrd);//  user typed login and password to registrate
};

#endif // NEWUSERWINDOW_H
