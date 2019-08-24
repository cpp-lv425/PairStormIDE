#include "startmanager.h"
#include "userslistmaker.h"
#include "choicewindow.h"
#include "downloaderWrapper.h"
#include "storeconf.h"
#include "newuserwindow.h"

#include <QDir>
#include <QUrl>
#include <QTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>
#include <QSettings>

//StartManager::StartManager(QObject *parent) : QObject(parent)
StartManager::StartManager(QWidget *parent) : QWidget(parent)
  , mUserMode {userMode::UnnamedUser}   // these four fields set for UnnamedUser mode
  , mUserName {"unnamed"}
  , mToken {40, '0'}
  , mIsTokenValid {false}
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
    //  B
    ChoiceWindow choiceWindow(mListRegisteredUsers, this);
    connect(&choiceWindow, &ChoiceWindow::cancel, this, [=](){emit cancel();});  // user pressed Cancel button in bottom-right side
    connect(&choiceWindow, &ChoiceWindow::rejected, this, [=](){emit cancel();});// user pressed Cross button in top-right side

    connect(&choiceWindow, &ChoiceWindow::choice, this, &StartManager::onChoice);
    connect(&choiceWindow, &ChoiceWindow::choice, &choiceWindow, &ChoiceWindow::accept);// need to terminate choiceWindow after user clicked some button

    choiceWindow.exec();
    qDebug() << "user" << mUserName << "token" << mToken << "mode" << static_cast<int>(mUserMode);

    //  C
    switch (mUserMode) {
    case userMode::UnnamedUser:
    {
        // fields [mUserName], [mToken] already set - nothing to do

        QEventLoop loop;
        QMessageBox *mbox = new QMessageBox;
        mbox->setWindowTitle(tr("Welcome to Pair Storm!"));
        QString msg {"You entering in unauthenticated mode"};
        mbox->setText(msg);
        mbox->setStandardButtons(nullptr);
        mbox->show();
        QTimer::singleShot(mTimeOutWelcomeWindow, this, [&](){
                loop.exit();
                mbox->accept();
            });
        loop.exec();

        break;
    }
    case userMode::RegisteredUser:
    {
        //  D
        validateToken();
        if (!mIsTokenValid)
        {
            QEventLoop loop;
            QMessageBox *mbox = new QMessageBox;
            mbox->setWindowTitle(tr("Welcome to Pair Storm!"));
            QString msg {mUserName + ", your credentials are not volid!"};
            mbox->setText(msg);
            mbox->setStandardButtons(nullptr);
            mbox->show();
            QTimer::singleShot(mTimeOutWelcomeWindow, this, [&](){
                    loop.exit();
                    mbox->accept();
                });
            loop.exec();

            emit cancel();

            break;
        }

        QEventLoop loop;
        QMessageBox *mbox = new QMessageBox;
        mbox->setWindowTitle(tr("Welcome to Pair Storm!"));
        QString msg {mUserName + ", your credentials are valid!"};
        mbox->setText(msg);
        mbox->setStandardButtons(nullptr);
        mbox->show();
        QTimer::singleShot(mTimeOutWelcomeWindow, this, [&](){
                loop.exit();
                mbox->accept();
            });
        loop.exec();

        break;
    }
    case userMode::NewUser:
    {   //  E
        // create new user window
        NewUserWindow newUserWindow(this);
        connect(&newUserWindow, &NewUserWindow::cancel, this, [=](){emit cancel();});  // user pressed Cancel button in bottom right side
        connect(&newUserWindow, &NewUserWindow::rejected, this, [=](){emit cancel();});// user pressed Cross button in top right side

        connect(&newUserWindow, &NewUserWindow::unnamedUser, this, &StartManager::onUnnamedUserChoice);
        connect(&newUserWindow, &NewUserWindow::unnamedUser, &newUserWindow, &NewUserWindow::accept);// need to terminate choiceWindow after user clicked button [Try without authentication]
        connect(&newUserWindow, &NewUserWindow::unnamedUser, this, [&](){
            QEventLoop loop;
            QMessageBox *mbox = new QMessageBox;
            mbox->setWindowTitle(tr("Welcome to Pair Storm!"));
            QString msg {"You entering in unauthenticated mode"};
            mbox->setText(msg);
            mbox->setStandardButtons(nullptr);
            mbox->show();
            QTimer::singleShot(mTimeOutWelcomeWindow, this, [&](){
                    loop.exit();
                    mbox->accept();
                });
            loop.exec();
                    });// and show welcpme window
        //connect(&newUserWindow, &NewUserWindow::choice, this, &StartManager::onChoice);
        //connect(&newUserWindow, &NewUserWindow::choice, &choiceWindow, &ChoiceWindow::accept);// need to terminate choiceWindow after user clicked some button

        connect(&newUserWindow, &NewUserWindow::newUserToken, this, &StartManager::onNewUserToken);// during registration user typed login and token
        connect(&newUserWindow, &NewUserWindow::newUserPasssword, this, &StartManager::onNewUserPassword);// during registration user typed login and password
        connect(this, &StartManager::cancelNewUserWindow, &newUserWindow, &NewUserWindow::accept);// token validated (or got new token) - close newUserWindow

        newUserWindow.exec();

        break;
    }
    default:
    {
        break;
    }
    }

    StoreConf conf(mUserName);
    conf.restoreConFile();
    if (mUserMode == userMode::NewUser)
    {   //qDebug() << "set token to new user";
        conf.setField("token", mToken);
        QSettings s;
        s.setValue("token", mToken);
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
            qDebug() << "login" << login;
        }
        if(mpDownloader->mRespondMap.contains("name"))
        {
            QString name = mpDownloader->mRespondMap["name"];
            qDebug() << "name" << name;
        }
        break;
    }
    case respondStatus::Error:
    {
        QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("An error while authorized is occured")
                             );
        emit cancel();
        break;
    }
    case respondStatus::Corrupted:
    {
        QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("An error while authorized is occured. Data corrupted")
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
    QString data = "{\"scopes\":[\"repo\",\"user\"],\"note\":\"" + mTokenPrefix + "\"}";
    QByteArray jsonString = "{\"scopes\":[\"repo\",\"user\"],\"note\":\"" + mTokenPrefix.toUtf8() + "\"}";
    //QByteArray jsonString = "{\"scopes\":[\"repo\",\"user\"],\"note\":\"getting-startedQ\"}";
    qDebug() << "mTokenPrefix" << mTokenPrefix;
    qDebug() << "QString data" << data;
    qDebug() << "QByteArray jsonString" << jsonString;

    mpDownloader->post2(mUserName, mUrlToGetToken, mPassword, jsonString);
    loop.exec();

    // analize answer
    switch (mRespondStatus)
    {
    case respondStatus::Normal:
    {
        if(mpDownloader->mRespondMap.contains("token"))
        {
            mToken = mpDownloader->mRespondMap["token"];
            mIsTokenValid = true;
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
        emit cancel();
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

void StartManager::onChoice(QString userName)
{
    if (userName == "unnamed")  // using application without registration
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
        // mUserName already set
        mToken = "";
        return;
    }
    QByteArray saveData = readFile.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(saveData);
    if(jsonDoc.isNull() || !jsonDoc.isObject() || jsonDoc.isEmpty()) //  QJsonDocument corrupted - need registration
    {
        mUserMode = userMode::NewUser;
        // mUserName already set
        mToken = "";
        return;
    }

    // in this stage file read successful and json document is valid
    QJsonObject json = jsonDoc.object();
    if (json.contains("token") && json["token"].isString())
    {
        mToken = json["token"].toString();
        mUserMode = userMode::RegisteredUser;
        mUserName = userName;
    }
    else                    //  not fount field [token] in configuration file
    {                       //  procedure like for new user
        mUserMode = userMode::NewUser;
        // mUserName already set
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
{
    //  F
    mUserName = login;
    mToken = token;
    validateToken();
    if (mIsTokenValid)
    {
        emit cancelNewUserWindow();

        QEventLoop loop;
        QMessageBox *mbox = new QMessageBox;
        mbox->setWindowTitle(tr("Welcome to Pair Storm!"));
        QString msg {mUserName + ", you are successfully registered!"};
        mbox->setText(msg);
        mbox->setStandardButtons(nullptr);
        mbox->show();
        QTimer::singleShot(mTimeOutWelcomeWindow, this, [&](){
                loop.exit();
                mbox->accept();
            });
        loop.exec();
    }
}

void StartManager::onNewUserPassword(const QString &login, const QString &password)
{
    //  G
    mUserName = login;
    mPassword = password;
    generateToken();
    if (mIsTokenValid)
    {
        emit cancelNewUserWindow();

        QEventLoop loop;
        QMessageBox *mbox = new QMessageBox;
        mbox->setWindowTitle(tr("Welcome to Pair Storm!"));
        QString msg {mUserName + ", you are successfully registered!"};
        mbox->setText(msg);
        mbox->setStandardButtons(nullptr);
        mbox->show();
        QTimer::singleShot(mTimeOutWelcomeWindow, this, [&](){
                loop.exit();
                mbox->accept();
            });
        loop.exec();
    }
}
