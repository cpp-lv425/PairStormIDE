#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QWidget>
#include <QPlainTextEdit>

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
    QString getTextFromConsole();
    void setTextToConsole(const QString &);


private:
    Ui::ConsoleWindow *ui;
};

#endif // CONSOLEWINDOW_H
