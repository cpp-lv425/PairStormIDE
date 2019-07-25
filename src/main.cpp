#include "mainwindow.h"

#include <QApplication>
#include <QTreeView>
#include <QDebug>

#include "projectviewermodel.h"
#include "startpage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.showStartPage();

    QStringList filters;
    filters << "*.txt"<<"*.cpp"<<"*.h"<<"*.json"<<"*.c"<<"*.hpp";
    QDir dir("/home/igor/Desktop/PairStormIDE/");
    ProjectViewerModel* fileSystemModel = new ProjectViewerModel(dir,filters);
    return a.exec();
}
