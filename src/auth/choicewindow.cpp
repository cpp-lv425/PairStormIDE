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
    // main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch(mBasicStretch);

    // widget parameters
    setModal(true);
    setMaximumSize(mWidgetWidth, mWidgetHeight);
    setStyle(QStyleFactory::create(mWidgetStyle));
    setStyleSheet(mWidgetColor);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // account's label parameters
    QLabel *mpLabelAccounts = new QLabel(mAccountsLabel, this);
    QString styleForAccounts = mLabelAccountsFontSize + mLabelAccountsFontFamily + mLabelAccountsFontStyle;
    mpLabelAccounts->setStyleSheet(styleForAccounts);
    mpLabelAccounts->setIndent(mLabelAccountsIndent);
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

    mpUsersWidgetList->setStyleSheet(mUsersListColor + mUsersListFontSize + mUsersListFontFamily);
    mpUsersWidgetList->setAutoFillBackground(true);
    mpUsersWidgetList->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(mpUsersWidgetList);
    connect(mpUsersWidgetList, &QListWidget::itemDoubleClicked, this, &ChoiceWindow::onListDoubleClicked);

    //      Buttons: New User Cancel
    QHBoxLayout *newUserCancelLayout = new QHBoxLayout;
    // New User Button parameters
    mpNewUserButton = new QPushButton(mNewUserLabel, this);
    mpNewUserButton->setMinimumWidth(mNewUserButtonWidth);
    mpNewUserButton->setMaximumWidth(mNewUserButtonWidth);
    QString styleNewUserButton = mNewUserButtonFontSize + mNewUserButtonFontFamily;
                               //+ mNewUserButtonColor + mNewUserButtonBackgroundColor;
    mpNewUserButton->setStyleSheet(styleNewUserButton);
    mpNewUserButton->setStyle(QStyleFactory::create(mWidgetStyle));

    //mainLayout->addWidget(mpNewUserButton);
    connect(mpNewUserButton, &QPushButton::clicked, this, &ChoiceWindow::onNewUserClicked);

    // Dialog button box (Cancel button) parameters
    mpButtonBox = new QDialogButtonBox(QDialogButtonBox::Cancel, this);
    QString styleButtonBox = mButtonBoxFontSize + mButtonBoxFontFamily;
    mpButtonBox->setStyleSheet(styleButtonBox);
    mpButtonBox->setStyle(QStyleFactory::create(mWidgetStyle));
    connect(mpButtonBox, &QDialogButtonBox::clicked, this, &ChoiceWindow::onBtnBoxClicked);

    newUserCancelLayout->addWidget(mpNewUserButton);
    newUserCancelLayout->addWidget(mpButtonBox);
    mainLayout->addLayout(newUserCancelLayout);

    // separator
    QFrame *line = new QFrame(this);
    line->setFrameShape(mSeparatorType);
    line->setFrameShadow(mSeparatorKind);
    line->setLineWidth(mSeparatorWidth);
    line->setStyleSheet(mSeparatorStyle);
    mainLayout->addWidget(line);

    // Unnamed User Button parameters
    QHBoxLayout *unnamedButtonLayout = new QHBoxLayout;
    mpUnnamedUserButton = new QPushButton(mUnnamedUserLabel, this);
    mpUnnamedUserButton->setFlat(true);
    mpUnnamedUserButton->setCursor(mUnnamedUserButtonCursor);
    mpUnnamedUserButton->setMinimumWidth(mUnnamedUserButtonWidth);
    mpUnnamedUserButton->setMaximumWidth(mUnnamedUserButtonWidth);
    QString styleUnnamedUserButton = mUnnamedUserButtonFontSize + mUnnamedUserButtonColor + mUnnamedUserButtonBorder;
    mpUnnamedUserButton->setStyleSheet(styleUnnamedUserButton);
    unnamedButtonLayout->addWidget(mpUnnamedUserButton, 0, mUnnamedUserButtonAlign);
    mainLayout->addLayout(unnamedButtonLayout);
    connect(mpUnnamedUserButton, &QPushButton::clicked, this, &ChoiceWindow::onUnnamedUserClicked);

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
