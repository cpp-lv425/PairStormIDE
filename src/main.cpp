#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "projectviewermodel.h"
#include "projecttreeview.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

    QStringList filters;
    filters << "*.txt"<<"*.cpp"<<"*.h"<<"*.json"<<"*.c"<<"*.hpp";
    QDir dir("C:\\Users\\igord\\Desktop\\PairStormIDE");
    ProjectViewerModel* fileSystemModel = new ProjectViewerModel(dir,filters);
    ProjectTreeView view(fileSystemModel);
    //view.setModel(fileSystemModel);
    view.setIconSize(QSize(20,28));
    view.setRootIndex(fileSystemModel->index(fileSystemModel->rootPath()));
    view.setVisible(true);
    view.hideColumn(1);
    view.hideColumn(2);
    view.hideColumn(3);
    view.show();

    return a.exec();
}
