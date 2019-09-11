/*
 * The purpose of the class is to authorize users.
 * It includes:
 *  - searching configuration files for registered users;
 *  - provide possibility to choice between registered users,
 *        continue as unregistered user and create new user;
 *  - handle registraton new user via github credentials or using existing token;
 *  - loading to QSettings parameters from configuration file;
 *        from userName.json for registeres users and unnamed.json for unregistered;
*/

#ifndef STARTMANAGER_H
#define STARTMANAGER_H

#include <QWidget>
#include <QMap>

class DownloaderWrapper;
class NewUserWindow;

class StartManager : public QWidget
{
    Q_OBJECT
public:
    explicit StartManager(QWidget *parent = nullptr);

    void start();                                       // to start authorization process
    void setPathToConfDir(const QString path);

    enum class userMode {RegisteredUser, NewUser, UnnamedUser, Size};
    enum class respondStatus {Normal, Error, Corrupted, Size};
private:
    DownloaderWrapper *mpDownloader;
    NewUserWindow *mNewUserWindow;

    userMode mUserMode                        = userMode::UnnamedUser;
    respondStatus mRespondStatus;

    QString mPathToConfDir;

    QString mUserName                         = "unnamed";
    const int mTokenSize                      = 40;
    const char mTokenPlaceholder              = '0';
    QString mToken                             {mTokenSize, '0'};
    QString mPassword;
    QString mTokenHash;
    QString mTokenPrefix                      = "PS";                       // prefix for generated token
    QByteArray mData;
    const QString mConfDirectoryName          = "conf";
    const QString mFileExtention              = ".json";

    bool mIsTokenValid                        = false;
    bool mIsTokenGenerated                    = false;

    QStringList mListRegisteredUsers;

    const QString mUrlToCheckUser             = "https://api.github.com/user";          //  Url to check validity token
    const QString mUrlToGetToken              = "https://api.github.com/authorizations";//  Url to generate new token

    void makeListRegisteredUsers();
    void choiceWindow();
    void validateToken();
    void generateToken();
    void newUsewWindow();
    void loadConFile();

    void formatTokenName(QString &tName);                                    // formatting token name as [tName] + currentSecsSinceEpoch
    void messageWindow(const QString &title, const QString &message, int timeOut);

    //  messages
    const QString mMessageTitle               = "Welcome to Pair Storm!";
    const QString mMessageRegistered          = ", you are successfully registered!";
    const QString mMessageUnauthenticated     = "You entering in unauthorized mode";
    const QString mMessageCredentialsValid    = ", your credentials are valid!";
    const QString mMessageCredentialsNotValid = ", your credentials are not valid!";
    const QString mMessageAccountAlreadyExist = "You already registered!";
    const QString mMessageBadLoginPassword    = "Your credentials not fits GitHub policy!";

    //              DESIGN PARAMETERS
    //  MessageWindow
    const int mMessageWindowFontSise          = 14;
    const bool mMessageWindowFontItalic       = false;
    const QFont::Weight mWeight               = QFont::Bold;
    const Qt::GlobalColor mMessageWindowColor = Qt::darkYellow;
    const QString mMessageWindowFont          = "Helvetica";
    const int mMessageWindowTimeOut           = 4000;                       // ms

signals:
    void cancel();                                                          // cancel mainwindow
    void cancelNewUserWindow();

public slots:
    void onChoice(QString userName);                                        // to set [mUserMode], [mUserName], [mToken] acording to user choise
    void onUnnamedUserChoice();                                             // user chose to continue as unregisteres user
    void onNewUserToken(const QString &login, const QString &token);        // to register user typed GiyHub credentials (login and password)
    void onNewUserPassword(const QString &login, const QString &pasword);   // to register user typed GiyHub login and token

};

#endif // STARTMANAGER_H
