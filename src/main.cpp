#include <QApplication>

#include "splashscreen.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SplashScreen splashScreen;
    splashScreen.start();

    MainWindow w;
    splashScreen.finish(&w);

    w.show();
    w.showStartPage();

    return a.exec();
}
