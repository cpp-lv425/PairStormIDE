#include "logindialog.h"

#include <QMessageBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>

LoginDialog::LoginDialog(QWidget *pParent): QDialog (pParent)
{
    setWindowTitle("Login");

    // create info label
    QLabel *pInfo = new QLabel;
    pInfo->setText("Please login to enable network sharing features");
    pInfo->setWordWrap(true);
    QFont infoFont("Segoe UI", 10);
    infoFont.setBold(true);
    pInfo->setFont(infoFont);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::blue);
    pInfo->setPalette(palette);

    // create user name label
    QLabel *pUserNameLbl = new QLabel("User Name");
    // create user name line edit
    mpUserNameLineEdit = new QLineEdit;
    mpUserNameLineEdit->setText("Unnamed");
    QHBoxLayout *pEditLayout = new QHBoxLayout;
    pEditLayout->addWidget(pUserNameLbl);
    pEditLayout->addWidget(mpUserNameLineEdit);

    // create group box to hold user name label & line edit
    QGroupBox *pLoginBox = new QGroupBox("Login");
    pLoginBox->setLayout(pEditLayout);

    // creating ok and cancel buttons
    QPushButton *pOk = new QPushButton("Ok");
    QPushButton *pCancel = new QPushButton("Cancel");
    QHBoxLayout *pButtonsLayout = new QHBoxLayout;
    pButtonsLayout->addStretch(1);
    pButtonsLayout->addWidget(pOk);
    pButtonsLayout->addWidget(pCancel);

    // connecting buttons
    connect(pCancel, &QPushButton::clicked, this, &LoginDialog::reject);
    connect(pOk, &QPushButton::clicked, this, &LoginDialog::onOkButtonClicked);

    // laying out window
    QVBoxLayout *pWdwLayout = new QVBoxLayout;
    pWdwLayout->addWidget(pInfo);
    pWdwLayout->addWidget(pLoginBox);
    pWdwLayout->addLayout(pButtonsLayout);
    setLayout(pWdwLayout);

    // specifying initial dialog size
    setGeometry(geometry().x(), geometry().y(), 400, 200);

    // centring dialog
    QPoint cntr = pParent->geometry().center();
    int x = cntr.x() - width() / 2;
    int y = cntr.y() - height() / 2;
    move(x, y);
}

QString LoginDialog::start()
{
    exec();
    // return user name
    return mUserName;
}

void LoginDialog::onOkButtonClicked()
{
    // check if user left empty edit line
    if (mpUserNameLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "User Name Input", "Please enter user name to proceed");
        return;
    }
    mUserName = mpUserNameLineEdit->text();
    accept();
}
