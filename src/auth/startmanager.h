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

//#include <QObject>
#include <QWidget>
#include <QMap>

class DownloaderWrapper;

//class StartManager : public QObject
class StartManager : public QWidget
{
    Q_OBJECT
public:
    //explicit StartManager(QObject *parent = nullptr);
    explicit StartManager(QWidget *parent = nullptr);

    void start();
    void setPathToConfDir(const QString path);

    enum class userMode {RegisteredUser, NewUser, UnnamedUser, Size};
    enum class respondStatus {Normal, Error, Corrupted, Size};
private:
    DownloaderWrapper *mpDownloader;

    userMode mUserMode;
    respondStatus mRespondStatus;

    QString mPathToConfDir;
    QString mUserName;
    QString mToken;
    QString mPassword;
    QString mTokenHash;
    QString mTokenPrefix    = "PS";
    QByteArray mData;

    bool mIsTokenValid;
    bool mIsTokenGenerated;

    QStringList mListRegisteredUsers;

    QString mUrlToCheckUser = "https://api.github.com/user";
    QString mUrlToGetToken  = "https://api.github.com/authorizations";

    void makeListRegisteredUsers();
    void choiceWindow();
    void validateToken();
    void generateToken();
    void newUsewWindow();
    void loadConFile();

    void formatTokenName(QString &name);     // formatting token name as name + currentSecsSinceEpoch

    int mTimeOutWelcomeWindow = 3000;        // ms
    void messageWindow(const QString &title, const QString &message, int timeOut);

signals:
    void cancel();                           // cancel mainwindow
    void cancelNewUserWindow();

public slots:
    void onChoice(QString userName);        // set [mUserMode], [mUserName], [mToken] acording to user choise
    void onUnnamedUserChoice();
    void onNewUserToken(const QString &login, const QString &token);
    void onNewUserPassword(const QString &login, const QString &pasword);

};

#endif // STARTMANAGER_H
