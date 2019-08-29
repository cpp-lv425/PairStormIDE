#include <QApplication>
#include <QQuickWindow>
#include <QWebEngineSettings>

#include "mainwindow.h"
#include "splashscreen.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SplashScreen splashScreen;
    splashScreen.start();

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);

    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);

    MainWindow w;
    splashScreen.finish(&w);

    w.show();
    w.showStartPage();

    return a.exec();
}
