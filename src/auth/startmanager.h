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
    QString mTokenHash;
    bool mIsTokenValid;
    QStringList mListRegisteredUsers;

    QString mUrlToCheckUser = "https://api.github.com/user";

    void makeListRegisteredUsers();
    void choiceWindow();
    void validateToken();
    void newUsewWindow();
    void loadConFile();

    int mTimeOutWelcomeWindow = 3000; // ms
signals:
    void cancel();
    void cancelNewUserWindow();

public slots:
    void onChoice(QString userName);
    void onNewUserChoice();
    void onNewUserToken(const QString &login, const QString &token);

};

#endif // STARTMANAGER_H
