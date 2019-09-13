#include <QApplication>
#include <QQuickWindow>

#include "mainwindow.h"
#include "splashscreen.h"
#include "startmanager.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);

    // Start manager must be started prior to both spalashscreen & mainwindow
    // because is is needed to choose application' settings source
    StartManager startManager;
    bool isManagerAborted = false;
    startManager.connect(&startManager, &StartManager::cancel, &startManager, [&]() {isManagerAborted = true;});
    startManager.start();
    if (isManagerAborted)
    {
        return 0;
    }

    SplashScreen splashScreen;
    splashScreen.start();
    MainWindow w;
    w.hide();
    splashScreen.finish(&w);
    w.show();
    w.showStartPage();

    return a.exec();
}
