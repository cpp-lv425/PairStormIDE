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

//class StartManager : public QObject
class StartManager : public QWidget
{
    Q_OBJECT
public:
    //explicit StartManager(QObject *parent = nullptr);
    explicit StartManager(QWidget *parent = nullptr);

    void start();
    void setPathToConfDir(const QString path);
private:
    enum class userMode {RegisteredUser, NewUser, UnnamedUser, Size};
    userMode mUserMode;
    QString mPathToConfDir;
    QString mUserName;
    QString mToken;
    QString mTokenHash;
    bool mIsTokenValid;
    QStringList mListRegisteredUsers;

    void makeListRegisteredUsers();
    void choiceWindow();
    void validateToken();
    void newUsewWindow();
    void loadConFile();


signals:
    void close();
public slots:
};

#endif // STARTMANAGER_H
