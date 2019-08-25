#include "choicewindow.h"
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

    QLabel *mpLabelAccounts = new QLabel("Avalable accounts", this);
    mainLayout->addWidget(mpLabelAccounts);

    mpUsersWidgetList = new QListWidget(this);
    QStringList usersList;
    for (int i = 0; i < usersFilesList.size(); ++i)// making users list based on configuration files
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

    mpNewUserButton = new QPushButton("New user", this);
    mainLayout->addWidget(mpNewUserButton);
    connect(mpNewUserButton, &QPushButton::clicked, this, &ChoiceWindow::onNewUserClicked);

    mpUnnamedUserButton = new QPushButton("Try without authorizattion", this);
    mainLayout->addWidget(mpUnnamedUserButton);
    connect(mpUnnamedUserButton, &QPushButton::clicked, this, &ChoiceWindow::onUnnamedUserClicked);

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
