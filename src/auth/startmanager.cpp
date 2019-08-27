/*
 *
 * in comments capital A B C D E F G H - lables in UML diagram
*/
#include "storeconf.h"
#include "startmanager.h"
#include "choicewindow.h"
#include "newuserwindow.h"
#include "userslistmaker.h"
#include "downloaderWrapper.h"

#include <QDir>
#include <QUrl>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QSettings>
#include <QEventLoop>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonDocument>

//StartManager::StartManager(QObject *parent) : QObject(parent)
StartManager::StartManager(QWidget *parent) : QWidget(parent)
  , mUserMode {userMode::UnnamedUser}   // these four fields set for UnnamedUser mode
  , mUserName {"unnamed"}
  , mToken {40, '0'}
  , mIsTokenValid {false}
  , mIsTokenGenerated {false}
{
    mpDownloader = new DownloaderWrapper(this);

    mPathToConfDir = QDir::currentPath();   // by default directory with configuration files
    mPathToConfDir += QDir::separator();        // along with executon file
    mPathToConfDir += "conf";
    mPathToConfDir += QDir::separator();
    //qDebug() << "mPathToConfDir" << mPathToConfDir;
}

void StartManager::start()
{
    //  A
    makeListRegisteredUsers();
    if (mListRegisteredUsers.isEmpty()) // switching to newUser window
    {
        mUserMode = userMode::NewUser;
        mUserName = "";
        mToken = "";
    }
    else //  B
    {
        ChoiceWindow choiceWindow(mListRegisteredUsers, this);
        connect(&choiceWindow, &ChoiceWindow::cancel, this, [&](){
                    mUserMode = userMode::Size;
                    emit cancel();
                });  // user pressed Cancel button in bottom-right side - termination mainwindow
        connect(&choiceWindow, &ChoiceWindow::rejected, this, [&](){
                    mUserMode = userMode::Size;
                    emit cancel();
                });// user pressed Cross button in top-right side - termination mainwindow

        connect(&choiceWindow, &ChoiceWindow::choice, this, &StartManager::onChoice);   // user choose one of three mode: registerdUser, newUser, unnamedUser
        connect(&choiceWindow, &ChoiceWindow::choice, &choiceWindow, &ChoiceWindow::accept);// need to terminate choiceWindow after user clicked some button

        choiceWindow.exec();
    }
    //qDebug() << "mUserMode" << static_cast<int>(mUserMode);
    //  C
    switch (mUserMode) {
    case userMode::UnnamedUser:
    {
        // fields [mUserName], [mToken] already set - nothing to do
        messageWindow("Welcome to Pair Storm!", "You entering in unauthenticated mode", mTimeOutWelcomeWindow);
        break;
    }
    case userMode::RegisteredUser:
    {
        //  D
        validateToken();
        if (mIsTokenValid)
        {
            messageWindow("Welcome to Pair Storm!", QString {mUserName + ", your credentials are valid!"}, mTimeOutWelcomeWindow);
            break;
        }

        messageWindow("Welcome to Pair Storm!", QString {mUserName + ", your credentials are not valid!"}, mTimeOutWelcomeWindow);
        //break; //don't need because when token not valid switching to newUser mode
    }
    case userMode::NewUser:
    {   //  E
        // create new user window
        NewUserWindow newUserWindow(this);
        connect(&newUserWindow, &NewUserWindow::cancel, this, [=](){emit cancel();});  // user pressed Cancel button in bottom right side
        connect(&newUserWindow, &NewUserWindow::rejected, this, [=](){emit cancel();});// user pressed Cross button in top right side

        // user clicked button [Try without authentication] - need to do three things:
        connect(&newUserWindow, &NewUserWindow::unnamedUser, this, &StartManager::onUnnamedUserChoice); // assign [mUserMode], [mUserName], [mToken]
        connect(&newUserWindow, &NewUserWindow::unnamedUser, &newUserWindow, &NewUserWindow::accept);   // terminate newUserWindow
        connect(&newUserWindow, &NewUserWindow::unnamedUser, this, [&](){                               // show notification window
            messageWindow("Welcome to Pair Storm!", "You entering in unauthenticated mode", mTimeOutWelcomeWindow);
                    });

        connect(&newUserWindow, &NewUserWindow::newUserToken, this, &StartManager::onNewUserToken);// during registration user typed login and token
        connect(&newUserWindow, &NewUserWindow::newUserPasssword, this, &StartManager::onNewUserPassword);// during registration user typed login and password
        connect(this, &StartManager::cancelNewUserWindow, &newUserWindow, &NewUserWindow::accept);// token validated (or got new token) - close newUserWindow

        newUserWindow.exec();

        break;
    }
    default:
    {   qDebug() << "default";
        mUserMode = userMode::UnnamedUser;
        mUserName = "unnamed";
        mToken = QString(40, '0');
        break;
    }
    }

    //  H
    StoreConf conf(mUserName);
    conf.restoreConFile();
    if (mUserMode == userMode::NewUser)     //  set token to new user
    {
        conf.setField("token", mToken);
        QSettings s;
        s.setValue("token", mToken);
    }
    else if (mUserMode == userMode::UnnamedUser) //  set token to unnamed user
    {
        conf.setField("token", QString(40, '0'));
        QSettings s;
        s.setValue("token", QString(40, '0'));
    }
}

void StartManager::setPathToConfDir(const QString path)
{
    mPathToConfDir = path;
}

void StartManager::makeListRegisteredUsers()
{
    mListRegisteredUsers = UsersListMaker::usersListMake(mPathToConfDir);
    //qDebug() << "list registered users" << mListRegisteredUsers;
}

void StartManager::validateToken()
{
    // GET request
    QEventLoop loop;
    connect(mpDownloader, &DownloaderWrapper::respond, [&](const QString &respond){
            if (respond == "normal")    mRespondStatus = respondStatus::Normal;
            if (respond == "error")     mRespondStatus = respondStatus::Error;
            if (respond == "corrupted") mRespondStatus = respondStatus::Corrupted;
            //qDebug() << "got respond" << respond;
            loop.exit();
                    });
    mpDownloader->get(mUserName, mUrlToCheckUser, mToken);
    loop.exec();

    // output
    qDebug() << "mRespondStatus" << static_cast<int>(mRespondStatus);
    qDebug() << "<Respond>";
    foreach (QString key, mpDownloader->mRespondMap.keys()) {
        auto value=mpDownloader->mRespondMap.value(key);
        qDebug() << key << ":" << value;
    }
    qDebug() << "</Respond>";
    qDebug() << "<Headers>";
    foreach (QString key, mpDownloader->mHeadersMap.keys()) {
        auto value=mpDownloader->mHeadersMap.value(key);
        qDebug() << key << ":" << value;
    }
    qDebug() << "</Headers>";
    // output
    // analize answer
    switch (mRespondStatus)
    {
    case respondStatus::Normal:
    {
        if(mpDownloader->mRespondMap.contains("login"))
        {
            QString login = mpDownloader->mRespondMap["login"];
            if (mUserName == login)
            {
                mIsTokenValid = true;
            }
        }
        if(mpDownloader->mRespondMap.contains("name"))
        {
            QString name = mpDownloader->mRespondMap["name"];
        }
        break;
    }
    case respondStatus::Error:
    {
        QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("An error while authorized is occured")
                             );
        break;
    }
    case respondStatus::Corrupted:
    {
        QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("An error while authorized is occured. Data corrupted")
                             );
        break;
    }
    default:
    {
        break;
    }
    }
}

void StartManager::generateToken()
{
    // POST request
    QEventLoop loop;
    connect(mpDownloader, &DownloaderWrapper::respond, [&](const QString &respond){
            if (respond == "normal")    mRespondStatus = respondStatus::Normal;
            if (respond == "error")     mRespondStatus = respondStatus::Error;
            if (respond == "corrupted") mRespondStatus = respondStatus::Corrupted;
            //qDebug() << "got respond" << respond;
            loop.exit();
                    });

    formatTokenName(mTokenPrefix);
    //QString mData = "{\"scopes\":[\"repo\",\"user\"],\"note\":\"" + mTokenPrefix + "\"}";
    QByteArray mData = "{\"scopes\":[\"repo\",\"user\"],\"note\":\"" + mTokenPrefix.toUtf8() + "\"}";
    //QByteArray jsonString = "{\"scopes\":[\"repo\",\"user\"],\"note\":\"getting-startedQ\"}";
    qDebug() << "mTokenPrefix" << mTokenPrefix;
    //qDebug() << "QString mData" << mData;
    //qDebug() << "QByteArray mData" << mData;

    mpDownloader->post(mUserName, mUrlToGetToken, mPassword, mData);
    loop.exec();

    qDebug() << "mRespondStatus" << static_cast<int>(mRespondStatus);

    // analize answer
    switch (mRespondStatus)
    {
    case respondStatus::Normal:
    {
        if(mpDownloader->mRespondMap.contains("token"))
        {
            mToken = mpDownloader->mRespondMap["token"];
            mIsTokenGenerated = true;
            qDebug() << "mToken" << mToken;
        }
        if(mpDownloader->mRespondMap.contains("note"))
        {
            QString note = mpDownloader->mRespondMap["note"];
            qDebug() << "note" << note;
        }
        break;
    }
    case respondStatus::Error:
    {
        QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("An error while token generation is occured")
                             );

        //emit cancel();
        break;
    }
    case respondStatus::Corrupted:
    {
        QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("An error while token generation is occured. Data corrupted")
                             );
        emit cancel();
        break;
    }
    default:
    {
        break;
    }
    }

}

void StartManager::formatTokenName(QString &name)
{
    auto epoch = QDateTime::currentSecsSinceEpoch();
    name += QString::number(epoch);
}

void StartManager::messageWindow(const QString &title, const QString &message, int timeOut)
{
    QEventLoop loop;
    QMessageBox *mbox = new QMessageBox;
    mbox->setWindowTitle(title);
    QString msg {message};
    mbox->setText(msg);
    mbox->setStandardButtons(nullptr);
    mbox->show();
    QTimer::singleShot(timeOut, this, [&](){
            loop.exit();
            mbox->accept();
        });
    loop.exec();
}

void StartManager::onChoice(QString userName)
{
    if (userName == "unnamed")  // using application without authentication
    {
        mUserMode = userMode::UnnamedUser;
        mUserName = userName;
        mToken    = QString(40, '0');
        return;
    }

    if (userName == "")         //  registration new user
    {
        mUserMode = userMode::NewUser;
        mUserName = "";
        mToken    = "";
        return;
    }

    //                              registered user
    // read token from configuration file
    QString file = mPathToConfDir + userName + ".json";
    QFile readFile(file);
    if(!readFile.open(QIODevice::ReadOnly)) //  file corrupted - need registration
    {
        mUserMode = userMode::NewUser;
        mUserName = userName;
        mToken = "";
        return;
    }
    QByteArray saveData = readFile.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(saveData);
    if(jsonDoc.isNull() || !jsonDoc.isObject() || jsonDoc.isEmpty()) //  QJsonDocument corrupted - need registration
    {
        mUserMode = userMode::NewUser;
        mUserName = userName;
        mToken = "";
        return;
    }

    // in this stage file read successful and json document is valid
    QJsonObject json = jsonDoc.object();
    if (json.contains("token") && json["token"].isString())
    {
        mUserMode = userMode::RegisteredUser;
        mUserName = userName;
        mToken = json["token"].toString();
    }
    else                    //  not fount field [token] in configuration file
    {                       //  procedure like for new user
        mUserMode = userMode::NewUser;
        mUserName = userName;
        mToken = "";
    }
}

void StartManager::onUnnamedUserChoice()
{
    mUserMode = userMode::UnnamedUser;
    mUserName = "unnamed";
    mToken = QString(40, '0');
}

void StartManager::onNewUserToken(const QString &login, const QString &token)
{   qDebug() << "onNewUserToken";
    //  F
    mUserName = login;
    mToken = token;
    validateToken();
    if (mIsTokenValid)
    {
        emit cancelNewUserWindow();

        messageWindow("Welcome to Pair Storm!", QString {mUserName + ", you are successfully registered!"}, mTimeOutWelcomeWindow);
    }
    else // in else case NewUserWindow asking to type credentials again
    {   // set all parameters for unnamedd user in case user will click cancel
        mUserMode = userMode::UnnamedUser;
        mUserName = "unnamed";
        mToken = QString(40, '0');
    }
}

void StartManager::onNewUserPassword(const QString &login, const QString &password)
{   qDebug() << "onNewUserPassword";
    //  G
    mUserName = login;
    mPassword = password;
    generateToken();
    if (mIsTokenGenerated)
    {
        emit cancelNewUserWindow();

        messageWindow("Welcome to Pair Storm!", QString {mUserName + ", you are successfully registered!"}, mTimeOutWelcomeWindow);
    }
    else // in else case NewUserWindow asking to type credentials again
    {   // set all parameters for unnamedd user in case user will click cancel
        mUserMode = userMode::UnnamedUser;
        mUserName = "unnamed";
        mToken = QString(40, '0');
    }
}

