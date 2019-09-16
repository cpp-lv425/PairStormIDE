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
    ConsoleServiceProvider* getConsoleServiceProvider() const;

public slots:
    void setProjectPath(QString);
    void reSendErrors(QString);
    void runExecutableFile();
signals:
    void errorsAreOccuredAfterCompilation(QString);
private:
    Ui::ConsoleWindow *ui;
    ConsoleServiceProvider *mpConsoleServiceProvider;
    ConsoleView *mpConsoleView;
};

#endif // CONSOLEWINDOW_H
