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

#include "mdiarea.h"
#include "browser.h"
#include "mainwindow.h"
#include "documentationsearch.h"
#include "documentationviewer.h"
#include "htmlcontentgenerator.h"
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);


    Browser *b = new Browser;
           b->newTab("array");
          // b.newTab("vector");
          // b.newTab("igorast");
           b->emptyDocumentationTab();
           //b.emptyDocumentationTab();
           b->show();
    return a.exec();
}
