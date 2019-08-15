#include "tabconnection.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QSettings>
#include <QLabel>

TabConnection::TabConnection(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addStretch(mBasicStretch);

    // getting data from QSettings
    QSettings s;
    mUDPportCurrent = (s.contains("defaultUdpPortNumber") ? s.value("defaultUdpPortNumber").toString() : mUDPportDefault);
    mTCPportCurrent = (s.contains("defaultTcpPortNumber") ? s.value("defaultTcpPortNumber").toString() : mTCPportDefault);
    mUDPportNew = mUDPportCurrent;
    mTCPportNew = mTCPportCurrent;
    mUserName = (s.contains("userName") ? s.value("userName").toString() : "");

    // user name
    QLabel *userLabel = new QLabel("User name", this);
    userLabel->setMinimumWidth(mLabelPortNumberWidth);
    userLabel->setMaximumWidth(mLabelPortNumberWidth);
    QHBoxLayout *userLayout = new QHBoxLayout;
    QLabel *userName = new QLabel(mUserName, this);
    userLayout->addWidget(userLabel);
    userLayout->addWidget(userName);
    userLayout->addStretch(mStretchFactor);
    mainLayout->addLayout(userLayout);
    mainLayout->addSpacing(mRowSpasing);

    // UDP port
    QLabel *UDPLabel = new QLabel("UDP port", this);
    UDPLabel->setMinimumWidth(mLabelPortNumberWidth);
    UDPLabel->setMaximumWidth(mLabelPortNumberWidth);
    QHBoxLayout *UDPLayout = new QHBoxLayout;
    mpUDPportEdit = new QLineEdit(mUDPportCurrent, this);
    mpUDPportEdit->setMaximumWidth(mLabelPortNumberWidth);
    UDPLayout->addWidget(UDPLabel);
    UDPLayout->addWidget(mpUDPportEdit);
    UDPLayout->addStretch(mStretchFactor);
    mainLayout->addLayout(UDPLayout);
    mainLayout->addSpacing(mRowSpasing);
    connect(mpUDPportEdit, &QLineEdit::editingFinished, this, &TabConnection::onUDPeditingFinished);

    // TCP port
    QLabel *TCPLabel = new QLabel("TCP port", this);
    TCPLabel->setMinimumWidth(mLabelPortNumberWidth);
    TCPLabel->setMaximumWidth(mLabelPortNumberWidth);
    QHBoxLayout *TCPLayout = new QHBoxLayout;
    mpTCPportEdit = new QLineEdit(mTCPportCurrent, this);
    mpTCPportEdit->setMaximumWidth(mLabelPortNumberWidth);
    TCPLayout->addWidget(TCPLabel);
    TCPLayout->addWidget(mpTCPportEdit);
    TCPLayout->addStretch(mStretchFactor);
    mainLayout->addLayout(TCPLayout);
    connect(mpTCPportEdit, &QLineEdit::editingFinished, this, &TabConnection::onTCPeditingFinished);


    mainLayout->addStretch(mStretchFactor);
    setLayout(mainLayout);
}

const QString &TabConnection::getUdpCurrernt() const
{
    return mUDPportCurrent;
}

const QString &TabConnection::getUdpNew() const
{
    return mUDPportNew;
}

void TabConnection::setUdpCurrernt(const QString & newPort)
{
    mUDPportCurrent = newPort;
}

const QString &TabConnection::getTcpCurrernt() const
{
    return mTCPportCurrent;
}

const QString &TabConnection::getTcpNew() const
{
    return mTCPportNew;
}

void TabConnection::setTcpCurrernt(const QString & newPort)
{
    mTCPportCurrent = newPort;
}

void TabConnection::onUDPeditingFinished()
{
    QString newPort = mpUDPportEdit->text();
    int port = newPort.toInt();
    if (port > 10000 && port < 65536 && newPort != mTCPportCurrent)
    {
        mUDPportNew = newPort;
    }
}
void TabConnection::onTCPeditingFinished()
{
    QString newPort = mpTCPportEdit->text();
    int port = newPort.toInt();
    if (port > 10000 && port < 65536 && newPort != mUDPportCurrent)
    {
        mTCPportNew = newPort;
    }
}
