#include "choicewindow.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>

ChoiceWindow::ChoiceWindow(QStringList &usersFilesList
                           , QString &userName
                           , QString &token
                           , QWidget *pParent)
    : QDialog (pParent)
{
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
            //continue;
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
}

void ChoiceWindow::onListDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "onListDoubleClicked";
}

void ChoiceWindow::onNewUserClicked()
{
    qDebug() << "onNewUserClicked";
}

void ChoiceWindow::onUnnamedUserClicked()
{
    qDebug() << "onUnnamedUserClicked";
}

void ChoiceWindow::onBtnBoxClicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton pressedButton;
    pressedButton = mpButtonBox->standardButton(button);

    if (pressedButton == QDialogButtonBox::Cancel)
    {
        emit close();
    }
}
