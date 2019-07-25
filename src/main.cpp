#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "projectviewermodel.h"
#include <QTreeView>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QStringList filters;
    filters << "*.txt"<<"*.cpp"<<"*.h"<<"*.json"<<"*.c"<<"*.hpp";
    QDir dir("/home/igor/Desktop/PairStormIDE/");
    ProjectViewerModel* fileSystemModel = new ProjectViewerModel(dir,filters);
    return a.exec();
}
