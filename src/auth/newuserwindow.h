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
#include <QFrame>

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
    explicit NewUserWindow(QStringList &registeredUsersList, QWidget *parent = nullptr);

    void unblockButtons();
private:
    QLabel *mWindowTitle;
    QLabel *mpLabelLogin;
    QLabel *mpLabelToken;
    QLineEdit *mpEditLogin;
    QLineEdit *mpEditToken;

    QPushButton *mpUnnamedUserButton;

    QDialogButtonBox *mpButtonBox;

    QStringList mRegisteredUsersList;

    //          DESIGN PARAMETERS
    //  Widget
    const int mWidgetWidth                          = 400;
    const int mWidgetHeight                         = 150;
    const QString mWidgetStyle                      = "Fusion";
    const QString mWidgetColor                      = "background-color: #b1c1c7;";
    int mBasicStretch                               = 1;

    //  Widget Title
    const QString mWidgetTitle                      = "Adding new user";
    const QString mWidgetTitleFontSize              = "font-size:   22px;";
    const QString mWidgetTitleFontFamily            = "font-family: Helvetica;";
    const QString mWidgetTitleFontStyle             = "font-style: bold;";
    const int mWidgetTitleIndent                    = 17;

    // login
    const QString mPlaceholderTextLogin             = "Your GitHub login";
    const QString mLoginLabel                       = "GitHub login";
    const int mLabelWidth                           = 135;
    const int mEditWidth                            = 320;

    const QString mLabelLoginFontSize               = "font-size:   16px;";
    const QString mLabelLoginFontFamily             = "font-family: Helvetica;";
    const QString mLabelLoginFontStyle              = "font-style: normal;";

    const QString mLineEditFontSize                 = "font-size: 16px;";
    const QString mLabelEditFontFamily              = "font-family: Helvetica;";
    const QString mLineEditColor                    = "background-color: #dce8eb;";

    // password/token
    const QString mPlaceholderTextToken             = "Sequence with 40 characters treats as token";
    const QString mTokenLabel                       = "Password or token";    
    const int mTokenSize                            = 40;

    //  dialog button box
    const QString mButtonBoxFontSize                = "font-size: 16px;";
    const QString mButtonBoxFontFamily              = "font-family: Helvetica;";
    const QString mButtonBoxColor                   = "color: rgb(0, 0, 0);";
    const QString mButtonBoxBackgroundColor         = "background-color: grey;";

    // separator
    const QString mSeparatorStyle                   = "background-color: #4e7b8a;";
    const int mSeparatorWidth                       = 2;
    const QFrame::Shape mSeparatorType              = QFrame::HLine;
    const QFrame::Shadow mSeparatorKind             = QFrame::Sunken;

    //  unnamed user button
    const QString mUnnamedUserLabel                 = "Try without authorization";
    const QString mUnnamedUserButtonFontSize        = "font-size: 13px;";
    const QString mUnnamedUserButtonFontFamily      = "font-family: Helvetica;";
    const QString mUnnamedUserButtonColor           = "color: #006199;";
    const QString mUnnamedUserButtonBorder          = "border: none;";
    const int mUnnamedUserButtonWidth               = 200;
    const Qt::AlignmentFlag mUnnamedUserButtonAlign = Qt::AlignRight;
    const Qt::CursorShape mUnnamedUserButtonCursor  = Qt::PointingHandCursor;

    bool checkInRegisteredUsers(const QString &newUser);
public slots:
    void onBtnBoxClicked(QAbstractButton * button);

signals:
    void cancel();                                                      //  user closed window - termination application
    void unnamedUser();                                                 //  user chose nunamed mode
    void newUserToken(const QString &login, const QString &token);      //  user typed login and token to registrate
    void newUserPasssword(const QString &login, const QString &passwrd);//  user typed login and password to registrate
    void accountAlreadyExist();                                         //  user typed login that already registered
    void badLoginPasswordTyped();                                       //  user typed login or/and password than not fits GitHub policy
};

#endif // NEWUSERWINDOW_H
