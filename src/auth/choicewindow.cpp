#include "choicewindow.h"
#include "downloaderWrapper.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleFactory>

ChoiceWindow::ChoiceWindow(const QStringList &usersFilesList, QWidget *pParent)
    : QDialog (pParent)
{
    // widget parameters
    setModal(true);
    setWindowTitle(mTitle);

    // Added "fusion" style
    setStyle(QStyleFactory::create("Fusion"));

    QPalette pal(palette());
    pal.setColor(mWindowColorRole, mWindowColor);
    setAutoFillBackground(true);
    setPalette(pal);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch(mBasicStretch);

    // account's label parameters
    QLabel *mpLabelAccounts = new QLabel(mAccountsLabel, this);
    QString styleForAccounts = mLabelAccountsFontSize + mLabelAccountsFontFamily + mLabelAccountsFontStyle;
    mpLabelAccounts->setStyleSheet(styleForAccounts);
    mainLayout->addWidget(mpLabelAccounts);

    // account's list parameters
    mpUsersWidgetList = new QListWidget(this);
    QStringList usersList;
    for (int i = 0; i < usersFilesList.size(); ++i) // creating users list based on configuration files
    {
        QString str(usersFilesList.at(i));
        str.chop(mFileExtention.size());
        if (str == "unnamed")
        {
            continue;
        }
        usersList << str;
    }
    mpUsersWidgetList->addItems(usersList);

    mpUsersWidgetList->setStyleSheet( mUsersListColor);
    mpUsersWidgetList->setAutoFillBackground(true);
    mpUsersWidgetList->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(mpUsersWidgetList);
    connect(mpUsersWidgetList, &QListWidget::itemDoubleClicked, this, &ChoiceWindow::onListDoubleClicked);

    // New User Button parameters
    mpNewUserButton = new QPushButton(mNewUserLabel, this);
    QString styleNewUserButton = mNewUserButtonFontSize + mNewUserButtonFontFamily
                               + mNewUserButtonColor + mNewUserButtonBackgroundColor;
    mpNewUserButton->setStyleSheet(styleNewUserButton);

    mainLayout->addWidget(mpNewUserButton);
    connect(mpNewUserButton, &QPushButton::clicked, this, &ChoiceWindow::onNewUserClicked);

    // Unnamed User Button parameters
    mpUnnamedUserButton = new QPushButton(mUnnamedUserLabel, this);
    QString styleUnnamedUserButton = mUnnamedUserButtonFontSize + mUnnamedUserButtonFontFamily
                                   + mUnnamedUserButtonColor + mUnnamedUserButtonBackgroundColor;
    mpUnnamedUserButton->setStyleSheet(styleUnnamedUserButton);
    mainLayout->addWidget(mpUnnamedUserButton);
    connect(mpUnnamedUserButton, &QPushButton::clicked, this, &ChoiceWindow::onUnnamedUserClicked);

    // Button box (Cancel button) parameters
    mpButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, this);
    QString styleButtonBox = mButtonBoxFontSize + mButtonBoxFontFamily
                           + mButtonBoxColor + mButtonBoxBackgroundColor;
    mpButtonBox->setStyleSheet(styleButtonBox);
    mainLayout->addWidget(mpButtonBox);
    connect(mpButtonBox, &QDialogButtonBox::clicked, this, &ChoiceWindow::onBtnBoxClicked);

    setLayout(mainLayout);
    show();

    // set widget in the middle of screen
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
