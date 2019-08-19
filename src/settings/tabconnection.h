#ifndef TABCONNECTION_H
#define TABCONNECTION_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE


class TabConnection : public QWidget
{
    Q_OBJECT

public:
    explicit TabConnection(QWidget *parent = nullptr);

    const QString & getUdpCurrernt() const;
    const QString & getUdpNew() const;
    void setUdpCurrernt(const QString &);

    const QString & getTcpCurrernt() const;
    const QString & getTcpNew() const;
    void setTcpCurrernt(const QString &);

private:
    // user name variables
    QString mUserName;

    // UDP port variables
    QString mUDPportCurrent;
    QString mUDPportDefault         = "36108";
    QString mUDPportNew;
    QLineEdit *mpUDPportEdit;

    // TCP port variables
    QString mTCPportCurrent;
    QString mTCPportDefault         = "32807";
    QString mTCPportNew;
    QLineEdit *mpTCPportEdit;

    // design parameters
    int mRowSpasing                 = 5;
    int mStretchFactor              = 20;
    int mLabelPortNumberWidth       = 80;
    int mLabelUserNameWidth         = 280;
    int mBasicStretch               = 1;
public slots:
    void onUDPeditingFinished();
    void onTCPeditingFinished();
};

#endif // TABCONNECTION_H
