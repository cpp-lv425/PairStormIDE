#include "bottompaneldock.h"

#include <QException>
#include <QTabWidget>

#include "mainwindow.h"

BottomPanelDock::BottomPanelDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle(tr("BottomPanel"));
    pTabWgt = new QTabWidget;

    // issues
    QWidget *pIssuesTab = new QWidget;
    pTabWgt->addTab(pIssuesTab, tr("Issues"));

    // compilation information
    QWidget *pCompileInfo = new QWidget;
    pTabWgt->addTab(pCompileInfo, tr("Compile output"));

    // debugging
    QWidget *pDebugConsole = new QWidget;
    pTabWgt->addTab(pDebugConsole, tr("Debugger Console"));

    // version control
    QWidget *pVersionsCtrl = new QWidget;
    pTabWgt->addTab(pVersionsCtrl, tr("Version Control"));

    setWidget(pTabWgt);
    pTabWgt->show();

    auto pMainWindow = qobject_cast<MainWindow*>(pParent);

    if(!pMainWindow)
        throw QException();

    pMainWindow->addDockWidget(Qt::BottomDockWidgetArea, this);
}
