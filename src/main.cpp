#include "mainwindow.h"

#include <QApplication>
#include <QTreeView>
#include <QDebug>

#include "projectviewermodel.h"
#include "projecttreeview.h"
#include "startpage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.showStartPage();

    return a.exec();
}
