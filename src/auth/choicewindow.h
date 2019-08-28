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
    const Qt::GlobalColor mWindowColor                  = Qt::lightGray;
    const QPalette::ColorRole mWindowColorRole          = QPalette::Background;
    const QString mFileExtention                        = ".json";
    const int mBasicStretch                             = 1;
    const QString mTitle                                = "Sign in, please";
    const QString mAccountsLabel                        = "Avalable accounts";
    const QString mNewUserLabel                         = "New user";
    const QString mUnnamedUserLabel                     = "Try without authorizattion";
    //  label accounts
    const QString mLabelAccountsFontSize                = "font-size: 16px;";
    const QString mLabelAccountsFontFamily              = "font-family: Helvetica;";
    const QString mLabelAccountsFontStyle               = "font-style: italic;";
    //  users list
    const QString mUsersListColor                       = "background: rgb(200, 200, 140);";
    //  new user button
    const QString mNewUserButtonFontSize                = "font-size: 16px;";
    const QString mNewUserButtonFontFamily              = "font-family: Helvetica;";
    const QString mNewUserButtonColor                   = "color: rgb(0, 0, 0);";
    const QString mNewUserButtonBackgroundColor         = "background-color: grey;";
    //  unnamed user button
    const QString mUnnamedUserButtonFontSize            = "font-size: 16px;";
    const QString mUnnamedUserButtonFontFamily          = "font-family: Helvetica;";
    const QString mUnnamedUserButtonColor               = "color: rgb(0, 0, 0);";
    const QString mUnnamedUserButtonBackgroundColor     = "background-color: grey;";
    //  dialog button box
    const QString mButtonBoxFontSize                    = "font-size: 16px;";
    const QString mButtonBoxFontFamily                  = "font-family: Helvetica;";
    const QString mButtonBoxColor                       = "color: rgb(0, 0, 0);";
    const QString mButtonBoxBackgroundColor             = "background-color: grey;";

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
