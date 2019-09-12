#include "newuserwindow.h"
#include "userslistmaker.h"
#include "validator.h"

#include<QVBoxLayout>
#include<QLabel>
#include<QLineEdit>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>
#include <QSettings>
#include <QStyleFactory>

NewUserWindow::NewUserWindow(QStringList &registeredUsersList, QWidget *parent)
    : QDialog(parent)
    , mRegisteredUsersList {registeredUsersList}
{
    // main layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch(mBasicStretch);

    // Widget parameters
    setModal(true);
    setMaximumSize(mWidgetWidth, mWidgetHeight);

    setStyle(QStyleFactory::create(mWidgetStyle));
    setStyleSheet(mWidgetColor);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // New user label
    mWindowTitle = new QLabel(mWidgetTitle, this);
    QString windowTitleStyleSheet = mWidgetTitleFontSize + mWidgetTitleFontFamily + mWidgetTitleFontStyle;
    mWindowTitle->setStyleSheet(windowTitleStyleSheet);
    mWindowTitle->setIndent(mWidgetTitleIndent);
    mainLayout->addWidget(mWindowTitle);

    //  login parameters
    QHBoxLayout *loginLayout = new QHBoxLayout;
    mpLabelLogin = new QLabel(mLoginLabel, this);
    mpLabelLogin->setMinimumWidth(mLabelWidth);
    mpLabelLogin->setMaximumWidth(mLabelWidth);
    QString styleForLabelLogin = mLabelLoginFontSize + mLabelLoginFontFamily + mLabelLoginFontStyle;
    mpLabelLogin->setStyleSheet(styleForLabelLogin);
    mpEditLogin = new QLineEdit(this);
    mpEditLogin->setMinimumWidth(mEditWidth);
    mpEditLogin->setMaximumWidth(mEditWidth);
    mpEditLogin->setStyleSheet(mLineEditColor + mLineEditFontSize + mLabelEditFontFamily);
    mpEditLogin->setToolTip(mPlaceholderTextLogin);
    loginLayout->addWidget(mpLabelLogin);
    loginLayout->addWidget(mpEditLogin);
    mainLayout->addLayout(loginLayout);

    //  token/password parameters
    QHBoxLayout *tokenLayout = new QHBoxLayout;
    mpLabelToken = new QLabel(mTokenLabel, this);
    mpLabelToken->setMinimumWidth(mLabelWidth);
    mpLabelToken->setMaximumWidth(mLabelWidth);
    mpLabelToken->setStyleSheet(styleForLabelLogin);
    mpEditToken = new QLineEdit(this);
    mpEditToken->setMinimumWidth(mEditWidth);
    mpEditToken->setMaximumWidth(mEditWidth);
    mpEditToken->setEchoMode(QLineEdit::Password);
    mpEditToken->setPlaceholderText(mPlaceholderTextToken);
    mpEditToken->setToolTip(mPlaceholderTextToken);
    mpEditToken->setStyleSheet(mLineEditColor);
    tokenLayout->addWidget(mpLabelToken);
    tokenLayout->addWidget(mpEditToken);
    mainLayout->addLayout(tokenLayout);

    // DialogButtonBox  parameters
    mpButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QString styleButtonBox = mButtonBoxFontSize + mButtonBoxFontFamily
                           + mButtonBoxColor + mButtonBoxBackgroundColor;
    mpButtonBox->setStyleSheet(styleButtonBox);
    connect(mpButtonBox, &QDialogButtonBox::clicked, this, &NewUserWindow::onBtnBoxClicked);
    mainLayout->addWidget(mpButtonBox);

    // separator
    QFrame *line = new QFrame(this);
    line->setFrameShape(mSeparatorType);
    line->setFrameShadow(mSeparatorKind);
    line->setLineWidth(mSeparatorWidth);
    line->setStyleSheet(mSeparatorStyle);
    mainLayout->addWidget(line);

    // unnamed user button
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
    connect(mpUnnamedUserButton, &QPushButton::clicked, this, [=](){emit unnamedUser();});

    setLayout(mainLayout);
    show();

    // set widget in the middle of screen
    move((QApplication::desktop()->width() - this->width()) / 2 ,
         (QApplication::desktop()->height() - this->height()) / 2);
}

void NewUserWindow::unblockButtons()
{
    mpButtonBox->setEnabled(true);
}

bool NewUserWindow::checkInRegisteredUsers(const QString &newUser)
{
    return mRegisteredUsersList.contains(newUser  + ".json");
}

void NewUserWindow::onBtnBoxClicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton pressedButton;
    pressedButton = mpButtonBox->standardButton(button);

    if (pressedButton == QDialogButtonBox::Cancel)
    {
        emit cancel();
        close();
    }

    if (pressedButton == QDialogButtonBox::Ok)
    {
        if (!mpEditToken->text().size() || !mpEditLogin->text().size())    // login or token not typed
        {
            return;
        }

        if (checkInRegisteredUsers(mpEditLogin->text()))                   // check if this account already exist
        {
           emit accountAlreadyExist();
           return;
        }

        /*if (!Validator::forGitHub(mpEditLogin->text(), mpEditToken->text()))// check GitHub' login/password policy
        {
            emit badLoginPasswordTyped();
            return;
        }*/

        if (mpEditToken->text().size() == mTokenSize && mpEditLogin->text().size())// user typed token
        {
            mpButtonBox->setEnabled(false);
            emit newUserToken(mpEditLogin->text(), mpEditToken->text());
            return;
        }
        if (mpEditToken->text().size() && mpEditLogin->text().size())               // user typed password
        {
            mpButtonBox->setEnabled(false);
            emit newUserPasssword(mpEditLogin->text(), mpEditToken->text());
            return;
        }
    }
}
