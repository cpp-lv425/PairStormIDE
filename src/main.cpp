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
    filters << "*.txt"<<"*.cpp"<<"*.h"<<"*.json"<<"*.c"<<"*.hpp";
    QDir dir("C:\\Users\\igord\\Desktop\\PairStormIDE");
    ProjectViewerModel* fileSystemModel = new ProjectViewerModel(dir,filters);
    view.setModel(fileSystemModel);
    view.setRootIndex(fileSystemModel->index(fileSystemModel->rootPath()));

    //view.sortByColumn(0,Qt::SortOrder::DescendingOrder);
    view.hideColumn(1);
    view.hideColumn(2);
    view.hideColumn(3);
    view.show();
    return a.exec();
}
