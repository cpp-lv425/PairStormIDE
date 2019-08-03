#include <QDir>
#include <QTime>
#include <QDebug>
#include <QApplication>
#include "connectionmanager.h"
#include <QStandardPaths>
#include <QTextStream>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <chrono>
#include <iostream>
#include <thread>
#include <QCommandLineParser>

#include "browser.h"
#include "splashscreen.h"

#include "mainwindow.h"
#include "documentationsearch.h"
#include "documentationviewer.h"
#include "htmlcontentgenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SplashScreen splashScreen;
    splashScreen.start();

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);


    Browser *browser = new Browser;

    MainWindow w;
   // w.setBrowser(browser);
    splashScreen.finish(&w);

    w.show();
    w.showStartPage();

    return a.exec();
}
