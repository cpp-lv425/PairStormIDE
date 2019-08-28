/*
 * visual widget provide to user posibility to choose between two mode:
 *      - to registerate new user:
 *          - via GitHub credentials (login and password);
 *          - via GitHub login and token;
 *      - procede as unregistered user with restricted permitions (unnamed user)
 *      or cancel app
*/
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

    QPushButton *mpUnnamedUserButton;

    QDialogButtonBox *mpButtonBox;

    //          DESIGN PARAMETERS
    const Qt::GlobalColor mWindowColor              = Qt::lightGray;
    const QPalette::ColorRole mWindowColorRole      = QPalette::Background;
    QString mPlaceholderText                        = "Sequence with 40 characters treats as token";
    const QString mTitle                            = "Create new account";
    const QString mLoginLabel                       = "GitHub login";
    const QString mTokenLabel                       = "Password or token";
    const QString mSeparatorStyle                   = "background-color: grey;";
    const QString mUnnamedUserLabel                 = "Try without authorizattion";
    const int mTokenSize                            = 40;

    int mLabelWidth                                 = 135;
    int mEditWidth                                  = 320;
    int mBasicStretch                               = 1;
    int mSeparatorWidth                             = 4;
    int mUnnamedUserButtonWidth                     = 200;

    //  LabelAccounts
    const QString mLabelLoginFontSize               = "font-size: 16px;";
    const QString mLabelLoginFontFamily             = "font-family: Helvetica;";
    const QString mLabelLoginFontStyle              = "font-style: italic;";

    //  LineEdit
    const QString mLineEditColor                    = "background: rgb(200, 200, 140);";

    //  unnamed user button
    const QString mUnnamedUserButtonFontSize        = "font-size: 16px;";
    const QString mUnnamedUserButtonFontFamily      = "font-family: Helvetica;";
    const QString mUnnamedUserButtonColor           = "color: rgb(0, 0, 0);";
    const QString mUnnamedUserButtonBackgroundColor = "background-color: grey;";

    //  dialog button box
    const QString mButtonBoxFontSize                = "font-size: 16px;";
    const QString mButtonBoxFontFamily              = "font-family: Helvetica;";
    const QString mButtonBoxColor                   = "color: rgb(0, 0, 0);";
    const QString mButtonBoxBackgroundColor         = "background-color: grey;";

public slots:
    void onBtnBoxClicked(QAbstractButton * button);

signals:
    void cancel();                                                      //  user closed window - termination application
    void unnamedUser();                                                 //  user chose nunamed mode
    void newUserToken(const QString &login, const QString &token);      //  user typed login and token to registrate
    void newUserPasssword(const QString &login, const QString &passwrd);//  user typed login and password to registrate
};

#endif // NEWUSERWINDOW_H
