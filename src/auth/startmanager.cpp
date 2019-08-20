#include "startmanager.h"
#include "userslistmaker.h"
#include "choicewindow.h"

#include <QDir>
#include <QDebug>

//StartManager::StartManager(QObject *parent) : QObject(parent)
StartManager::StartManager(QWidget *parent) : QWidget(parent)
{
    mPathToConfDir = QDir::currentPath();   // by default directory with configuration files
    mPathToConfDir += QDir::separator();        // along with executon file
    mPathToConfDir += "conf";
    mPathToConfDir += QDir::separator();
    qDebug() << "mPathToConfDir" << mPathToConfDir;
}

void StartManager::setPathToConfDir(const QString path)
{
    mPathToConfDir = path;
}

void StartManager::makeListRegisteredUsers()
{
    mListRegisteredUsers = UsersListMaker::usersListMake(mPathToConfDir);
    qDebug() << "list registered users" << mListRegisteredUsers;
}

void StartManager::start()
{
    makeListRegisteredUsers();
    ChoiceWindow choiceWindow(mListRegisteredUsers, mUserName, mToken);
    //connect(&choiceWindow, &ChoiceWindow::close, this, &StartManager::close);
    connect(&choiceWindow, &ChoiceWindow::close, this, [=](){emit close();});
    choiceWindow.exec();
}
