/*
 * This class manage user's authorization via GitHub token or GitHub credentials
 * If user's account exist - it loading configuration file to application settings
 * If it is registration - creates one and loading.
 *
 * In comments capital A B C D E F G H - labels in UML diagram
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
#include <QStyleFactory>

StartManager::StartManager(QWidget *parent) : QWidget(parent)
{
    mpDownloader = new DownloaderWrapper(this);

    mPathToConfDir = QDir::currentPath();   // by default directory with configuration files
    mPathToConfDir += QDir::separator();        // along with executon file
    mPathToConfDir += mConfDirectoryName;
    mPathToConfDir += QDir::separator();
}

void StartManager::start()
{
    //  A
    makeListRegisteredUsers();
    if (mListRegisteredUsers.isEmpty())     // in C section switching to newUser window
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

    //  C
    switch (mUserMode) {
    case userMode::UnnamedUser:
    {
        mUserName = "unnamed";
        mToken    = QString(mTokenSize, mTokenPlaceholder);
        //messageWindow(mMessageTitle, mMessageUnauthenticated, mMessageWindowTimeOut);
        break;
    }
    case userMode::RegisteredUser:
    {
        //  D
        validateToken();
        if (mIsTokenValid)
        {
            //messageWindow(mMessageTitle, QString {mUserName + mMessageCredentialsValid}, mMessageWindowTimeOut);
            break;
        }

        //messageWindow(mMessageTitle, QString {mUserName + mMessageCredentialsNotValid}, mMessageWindowTimeOut);
        //break don't need because when token not valid switching to newUser mode
    }
    case userMode::NewUser:
    {   //  E
        // create new user window mNewUserWindow
        mNewUserWindow = new NewUserWindow(mListRegisteredUsers ,this);
        connect(mNewUserWindow, &NewUserWindow::accountAlreadyExist, this, [&](){messageWindow(mMessageTitle, mMessageAccountAlreadyExist, mMessageWindowTimeOut);});  // user typed login that already exist
        connect(mNewUserWindow, &NewUserWindow::badLoginPasswordTyped, this, [&](){messageWindow(mMessageTitle, mMessageBadLoginPassword, mMessageWindowTimeOut);});  // user typed login/password that not fits GitHub policy
        connect(mNewUserWindow, &NewUserWindow::cancel, this, [=](){emit cancel();});  // user pressed Cancel button in bottom right side
        connect(mNewUserWindow, &NewUserWindow::rejected, this, [=](){emit cancel();});// user pressed Cross button in top right side

        // user clicked button [Try without authentication] - need to do three things:
        connect(mNewUserWindow, &NewUserWindow::unnamedUser, this, &StartManager::onUnnamedUserChoice); // assign [mUserMode], [mUserName], [mToken]
        connect(mNewUserWindow, &NewUserWindow::unnamedUser, mNewUserWindow, &NewUserWindow::accept);   // terminate newUserWindow
        //connect(mNewUserWindow, &NewUserWindow::unnamedUser, this, [&](){                               // show notification window
        //    messageWindow(mMessageTitle, mMessageUnauthenticated, mMessageWindowTimeOut);
        //            });

        connect(mNewUserWindow, &NewUserWindow::newUserToken, this, &StartManager::onNewUserToken);// during registration user typed login and token
        connect(mNewUserWindow, &NewUserWindow::newUserPasssword, this, &StartManager::onNewUserPassword);// during registration user typed login and password
        connect(this, &StartManager::cancelNewUserWindow, mNewUserWindow, &NewUserWindow::accept);// token validated (or got new token) - close newUserWindow

        mNewUserWindow->exec();

        break;
    }
    default:
    {
        mUserMode = userMode::UnnamedUser;
        mUserName = "unnamed";
        mToken = QString(mTokenSize, mTokenPlaceholder);
        break;
    }
    }

    //  H
    if (!mUserName.size())                  //  prevent creation configuration file
    {                                       //     with empty name in case user pushed Cancel
        mUserName = "unnamed";
    }

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
        conf.setField("token", QString(mTokenSize, mTokenPlaceholder));
        QSettings s;
        s.setValue("token", QString(mTokenSize, mTokenPlaceholder));
    }
}

void StartManager::setPathToConfDir(const QString path)
{
    mPathToConfDir = path;
}

void StartManager::makeListRegisteredUsers()
{
    mListRegisteredUsers = UsersListMaker::usersListMake(mPathToConfDir);
}

void StartManager::validateToken()
{
    // GET request
    QEventLoop loop;
    connect(mpDownloader, &DownloaderWrapper::respond, [&](const QString &respond){
            if (respond == "normal")    mRespondStatus = respondStatus::Normal;
            if (respond == "error")     mRespondStatus = respondStatus::Error;
            if (respond == "corrupted") mRespondStatus = respondStatus::Corrupted;
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
            else
            {
                messageWindow(mMessageTitle, mUserName + mMessageCredentialsNotValid, mMessageWindowTimeOut);
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
        /*QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("An error while authorized is occured")
                             );*/
        messageWindow(mMessageTitle, mMessageErrorInRequest, mMessageWindowTimeOut);
        break;
    }
    case respondStatus::Corrupted:
    {
        /*QMessageBox::critical(nullptr,
                              tr("Error"),
                              tr("An error while authorized is occured. Data corrupted")
                             );*/
        messageWindow(mMessageTitle, mMessageErrorInRequestCorrupt, mMessageWindowTimeOut);
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
            loop.exit();
                    });

    formatTokenName(mTokenPrefix);
    QByteArray mData = "{\"scopes\":[\"repo\",\"user\"],\"note\":\"" + mTokenPrefix.toUtf8() + "\"}";
    mpDownloader->post(mUserName, mUrlToGetToken, mPassword, mData);
    loop.exec();

    // analize answer
    switch (mRespondStatus)
    {
    case respondStatus::Normal:
    {
        if(mpDownloader->mRespondMap.contains("token"))
        {
            mToken = mpDownloader->mRespondMap["token"];
            mIsTokenGenerated = true;
        }
        else
        {
            messageWindow(mMessageTitle, mUserName + mMessageCredentialsNotValid, mMessageWindowTimeOut);
        }
        if(mpDownloader->mRespondMap.contains("note"))
        {
            QString note = mpDownloader->mRespondMap["note"];
        }
        break;
    }
    case respondStatus::Error:
    {
        messageWindow(mMessageTitle, mMessageErrorInRequest, mMessageWindowTimeOut);
        break;
    }
    case respondStatus::Corrupted:
    {
        messageWindow(mMessageTitle, mMessageErrorInRequestCorrupt, mMessageWindowTimeOut);
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
    QMessageBox *mbox = new QMessageBox;mbox->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    mbox->setWindowTitle(title);
    QString msg {message};
    mbox->setText(msg);
    mbox->setStandardButtons(nullptr);

    //const QString mWidgetStyle                      = "Fusion";
    //setStyle(QStyleFactory::create(mWidgetStyle));
    // set background color
    //QPalette palette;
    //palette.setColor(QPalette::Background, mMessageWindowColor);
    //mbox->setPalette(palette);
    QString styleMessageWindow = mMessageWindowColor;
    mbox->setStyleSheet(styleMessageWindow);

    // set font
    QFont font(mMessageWindowFont);
    font.setPointSize(mMessageWindowFontSise);
    font.setWeight(mWeight);
    font.setItalic(mMessageWindowFontItalic);
    mbox->setFont(font);

    mbox->show();

    // set timeout
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
        mToken    = QString(mTokenSize, mTokenPlaceholder);
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
    QString file = mPathToConfDir + userName + mFileExtention;
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
    mToken = QString(mTokenSize, mTokenPlaceholder);
}

void StartManager::onNewUserToken(const QString &login, const QString &token)
{
    //  F
    mUserName = login;
    mToken = token;
    validateToken();

    // unblock OK Cancel buttons in New User Window
    mNewUserWindow->unblockButtons();

    if (mIsTokenValid)
    {
        emit cancelNewUserWindow();

        messageWindow(mMessageTitle, QString {mUserName + mMessageRegistered}, mMessageWindowTimeOut);
    }
    else // in else case NewUserWindow asking to type credentials again
    {   // set all parameters for unnamedd user in case user will click cancel
        //messageWindow(mMessageTitle, QString {mUserName + mMessageCredentialsNotValid}, mMessageWindowTimeOut);
        mUserMode = userMode::UnnamedUser;
        mUserName = "unnamed";
        mToken = QString(mTokenSize, mTokenPlaceholder);
    }
}

void StartManager::onNewUserPassword(const QString &login, const QString &password)
{
    //  G
    mUserName = login;
    mPassword = password;
    generateToken();

    // unblock OK Cancel buttons in New User Window
    mNewUserWindow->unblockButtons();

    if (mIsTokenGenerated)
    {
        emit cancelNewUserWindow();

        messageWindow(mMessageTitle, QString {mUserName + mMessageRegistered}, mMessageWindowTimeOut);
    }
    else // in else case NewUserWindow asking to type credentials again
    {   // set all parameters for unnamed user in case user will click cancel
        mUserMode = userMode::UnnamedUser;
        mUserName = "unnamed";
        mToken = QString(mTokenSize, mTokenPlaceholder);
    }
}

