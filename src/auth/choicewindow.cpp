#include "choicewindow.h"
//#include "startmanager.h"       // to use enum class userMode
#include "downloaderWrapper.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

ChoiceWindow::ChoiceWindow(QStringList &usersFilesList, QWidget *pParent)
    : QDialog (pParent)
{
    window()->setFixedSize( window()->sizeHint() );
    setModal(true);
    setWindowTitle("Sign in, please");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch(mBasicStretch);

    mpLabelAccounts = new QLabel("Avalable accounts", this);
    mainLayout->addWidget(mpLabelAccounts);

    mpUsersWidgetList = new QListWidget(this);
    QStringList usersList;
    for (int i = 0; i < usersFilesList.size(); ++i)
    {
        QString str(usersFilesList.at(i));
        str.chop(5);
        if (str == "unnamed")
        {
            continue;
        }
        usersList << str;
    }
    mpUsersWidgetList->addItems(usersList);
    mpUsersWidgetList->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(mpUsersWidgetList);
    connect(mpUsersWidgetList, &QListWidget::itemDoubleClicked, this, &ChoiceWindow::onListDoubleClicked);

    mpNewUser = new QPushButton("New user", this);
    mainLayout->addWidget(mpNewUser);
    connect(mpNewUser, &QPushButton::clicked, this, &ChoiceWindow::onNewUserClicked);

    mpUnnamedUser = new QPushButton("Try without authorizattion", this);
    mainLayout->addWidget(mpUnnamedUser);
    connect(mpUnnamedUser, &QPushButton::clicked, this, &ChoiceWindow::onUnnamedUserClicked);

    mpButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(mpButtonBox);
    connect(mpButtonBox, &QDialogButtonBox::clicked, this, &ChoiceWindow::onBtnBoxClicked);

    setLayout(mainLayout);
    show();
    move((QApplication::desktop()->width() - this->width()) / 2 ,
                   (QApplication::desktop()->height() - this->height()) / 2);
}

void ChoiceWindow::onListDoubleClicked(QListWidgetItem *item)
{
    emit choice(item->text());
    close();
}

void ChoiceWindow::onNewUserClicked()
{
    emit choice("");
    close();
    //DownloaderGui downloader(this);
    //downloader.onButtonGET();
}

void ChoiceWindow::onUnnamedUserClicked()
{
    emit choice("unnamed");
    close();
}

void ChoiceWindow::onBtnBoxClicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton pressedButton;
    pressedButton = mpButtonBox->standardButton(button);

    if (pressedButton == QDialogButtonBox::Cancel)
    {
        emit cancel();
        close();
    }
}
