#include <QApplication>
#include <QQuickWindow>

#include "mainwindow.h"
#include "splashscreen.h"
#include "startmanager.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SplashScreen splashScreen;
    splashScreen.start();

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);

    MainWindow w;
    w.hide();
    splashScreen.finish(&w);
    StartManager startManager(&w);
    w.connect(&startManager, &StartManager::cancel, &w, [&]() {w.mIsFinished = true;});
    startManager.start();
    if (w.mIsFinished)
    {
        return 0;
    }
    w.show();
    w.showStartPage();

    return a.exec();
}
