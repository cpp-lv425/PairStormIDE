#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QObject>
#include <QWidget>

class ConsoleWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleWindow(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // CONSOLEWINDOW_H
