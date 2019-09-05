#ifndef CONSOLESERVICEPROVIDER_H
#define CONSOLESERVICEPROVIDER_H

#include <QObject>

class ConsoleServiceProvider: public QObject
{
    Q_OBJECT
public:
    ConsoleServiceProvider();
    virtual ~ConsoleServiceProvider();
};

#endif // CONSOLESERVICEPROVIDER_H
