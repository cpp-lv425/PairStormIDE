#ifndef USERSBACKEND_H
#define USERSBACKEND_H

#include <QObject>

class AvailableUsersModel : public QObject
{
    Q_OBJECT
public:
    explicit AvailableUsersModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // USERSBACKEND_H
