#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QWidget>

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

private:
    Ui::ConsoleWindow *ui;
};

#endif // CONSOLEWINDOW_H
