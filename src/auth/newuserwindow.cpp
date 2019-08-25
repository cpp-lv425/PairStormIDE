#include "newuserwindow.h"

#include<QVBoxLayout>
#include<QLabel>
#include<QLineEdit>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>
#include <QToolTip>

NewUserWindow::NewUserWindow(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    window()->setFixedSize( window()->sizeHint() );
    setWindowTitle("Create new account");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch(mBasicStretch);

    //  login parameters
    QHBoxLayout *loginLayout = new QHBoxLayout;
    mpLabelLogin = new QLabel("GitHub login", this);
    mpLabelLogin->setMinimumWidth(mLabelWidth);
    mpLabelLogin->setMaximumWidth(mLabelWidth);
    mpEditLogin = new QLineEdit(this);
    mpEditLogin->setMinimumWidth(mEditWidth);
    mpEditLogin->setMaximumWidth(mEditWidth);
    loginLayout->addWidget(mpLabelLogin);
    loginLayout->addWidget(mpEditLogin);
    mainLayout->addLayout(loginLayout);

    //  token/password parameters
    QHBoxLayout *tokenLayout = new QHBoxLayout;
    mpLabelToken = new QLabel("Password or token", this);
    mpLabelToken->setMinimumWidth(mLabelWidth);
    mpLabelToken->setMaximumWidth(mLabelWidth);
    mpEditToken = new QLineEdit(this);
    mpEditToken->setMinimumWidth(mEditWidth);
    mpEditToken->setMaximumWidth(mEditWidth);
    mpEditToken->setEchoMode(QLineEdit::Password);
    mpEditToken->setPlaceholderText(mPlaceholderText);
    mpEditToken->setToolTip(mPlaceholderText);
    tokenLayout->addWidget(mpLabelToken);
    tokenLayout->addWidget(mpEditToken);
    mainLayout->addLayout(tokenLayout);

    // DialogButtonBox  parameters
    mpButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    //mpButtonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
    connect(mpButtonBox, &QDialogButtonBox::clicked, this, &NewUserWindow::onBtnBoxClicked);
    mainLayout->addWidget(mpButtonBox);

    // separator
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(mSeparatorWidth);
    mainLayout->addWidget(line);

    // unnamed user button
    QHBoxLayout *unnamedButtonLayout = new QHBoxLayout;
    mpUnnamedUserButton = new QPushButton("Try without authorizattion", this);
    mpUnnamedUserButton->setMinimumWidth(mUnnamedUserButtonWidth);
    mpUnnamedUserButton->setMaximumWidth(mUnnamedUserButtonWidth);
    unnamedButtonLayout->addWidget(mpUnnamedUserButton);
    mainLayout->addLayout(unnamedButtonLayout);
    connect(mpUnnamedUserButton, &QPushButton::clicked, this, [=](){emit unnamedUser();});
    setLayout(mainLayout);
    show();
    move((QApplication::desktop()->width() - this->width()) / 2 ,
         (QApplication::desktop()->height() - this->height()) / 2);
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
        if (mpEditToken->text().size() == 40 && mpEditLogin->text().size())// user typed token
        {
            emit newUserToken(mpEditLogin->text(), mpEditToken->text());
            return;
        }
        if (mpEditToken->text().size() && mpEditLogin->text().size())      // user typed password
        {
            emit newUserPasssword(mpEditLogin->text(), mpEditToken->text());
            return;
        }
    }
}
