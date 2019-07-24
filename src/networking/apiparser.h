#ifndef APIPARSER_H
#define APIPARSER_H

#include <QObject>

class ApiParser : public QObject
{
    Q_OBJECT
public:
    explicit ApiParser(QObject *parent = nullptr);

signals:

public slots:
};

#endif // APIPARSER_H
