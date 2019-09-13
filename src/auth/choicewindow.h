/*
 * visual widget provide to user posibility to choose between three mode:
 *      - choose one of registered users;
 *      - to registerate new user;
 *      - procede as unregistered user with restricted permitions (unnamed user)
 *      or cancel app
*/
#ifndef CHOICEWINDOW_H
#define CHOICEWINDOW_H

#include <QDialog>
#include <QFont>
#include <QFrame>

QT_BEGIN_NAMESPACE
//class QFont;
class QLabel;
class QListWidget;
class QAbstractButton;
class QListWidgetItem;
class QDialogButtonBox;
QT_END_NAMESPACE

class ChoiceWindow : public QDialog
{
    Q_OBJECT
public:
    ChoiceWindow(const QStringList &usersFilesList, QWidget *pParent = nullptr);

private:
    QListWidget *mpUsersWidgetList;

    QPushButton *mpUnnamedUserButton;
    QPushButton *mpNewUserButton;

    QDialogButtonBox *mpButtonBox;    

    //          DESIGN PARAMETERS
    //  Widget
    const int mWidgetWidth                              = 600;
    const int mWidgetHeight                             = 300;
    const QString mWidgetStyle                          = "Fusion";
    const QString mWidgetColor                          = "background-color: #b1c1c7;";
    int mBasicStretch                                   = 1;


    const QString mFileExtention                        = ".json";

    //  label accounts
    const QString mAccountsLabel                        = "Avalable accounts";
    const QString mLabelAccountsFontSize                = "font-size: 22px;";
    const QString mLabelAccountsFontFamily              = "font-family: Helvetica;";
    const QString mLabelAccountsFontStyle               = "font-style: bold;";
    const int mLabelAccountsIndent                      = 17;

    //  users list
    const QString mUsersListColor                       = "background-color: #dce8eb;";
    const QString mUsersListFontSize                    = "font-size: 16px;";
    const QString mUsersListFontFamily                  = "font-family: Helvetica;";
    //  new user button
    const int mNewUserButtonWidth                       = 80;
    const QString mNewUserButtonFontSize                = "font-size: 13px;";
    const QString mNewUserButtonFontFamily              = "font-family: Helvetica;";
    const QString mNewUserButtonColor                   = "color: rgb(0, 0, 0);";
    const QString mNewUserButtonBackgroundColor         = "background-color: grey;";
    const QString mNewUserLabel                         = "New user";

    //  dialog button box
    const QString mButtonBoxFontSize                    = "font-size: 13px;";
    const QString mButtonBoxFontFamily                  = "font-family: Helvetica;";
    const QString mButtonBoxColor                       = "color: rgb(0, 0, 0);";
    const QString mButtonBoxBackgroundColor             = "background-color: grey;";

    // separator
    const QString mSeparatorStyle                       = "background-color: #4e7b8a;";
    const int mSeparatorWidth                           = 2;
    const QFrame::Shape mSeparatorType                  = QFrame::HLine;
    const QFrame::Shadow mSeparatorKind                 = QFrame::Sunken;
    //  unnamed user button
    const QString mUnnamedUserLabel                     = "Try without authorization";
    const QString mUnnamedUserButtonFontSize            = "font-size: 13px;";
    const QString mUnnamedUserButtonFontFamily          = "font-family: Helvetica;";
    const QString mUnnamedUserButtonColor               = "color: #006199;";
    const QString mUnnamedUserButtonBorder              = "border: none;";
    const int mUnnamedUserButtonWidth                   = 200;
    const Qt::AlignmentFlag mUnnamedUserButtonAlign     = Qt::AlignRight;
    const Qt::CursorShape mUnnamedUserButtonCursor      = Qt::PointingHandCursor;
private slots:
    void onListDoubleClicked(QListWidgetItem *item);
    void onNewUserClicked();
    void onUnnamedUserClicked();
    void onBtnBoxClicked(QAbstractButton *button);

signals:
    void cancel();
    void choice(QString userName);

};

#endif // CHOICEWINDOW_H
