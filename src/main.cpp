#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "projectviewermodel.h"
#include <QTreeView>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    QTreeView view;
    QStringList filters;
    filters << "*.txt"<<"*.cpp"<<".h";
    QDir dir("/home/igor/Desktop/PairStormIDE/");
    ProjectViewerModel* fileSystemModel = new ProjectViewerModel(dir,filters);
    view.setModel(fileSystemModel);
    view.show();
    return a.exec();
}
