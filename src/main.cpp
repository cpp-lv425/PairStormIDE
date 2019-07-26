#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "projectviewermodel.h"
#include "projecttreeview.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QStringList filters;
    filters << "*.txt"<<"*.cpp"<<"*.h"<<"*.json"<<"*.c"<<"*.hpp";

    QDir dir("C:\\Users\\igord\\Desktop\\PairStormIDE");
    ProjectViewerModel* fileSystemModel = new ProjectViewerModel(dir,filters);
    ProjectTreeView view(fileSystemModel);
    view.show();

    return a.exec();
}
