#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QWidget>
#include "consoleserviceprovider.h"
#include "consoleview.h"

namespace Ui
{
class ConsoleWindow;
}

class ConsoleWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleWindow(QWidget *parent = nullptr);
    ~ConsoleWindow();
public slots:
    void setProjectPath(QString);
private:
    Ui::ConsoleWindow *ui;
    ConsoleServiceProvider *consoleServiceProvider;
    ConsoleView *consoleView;
};

#endif // CONSOLEWINDOW_H
