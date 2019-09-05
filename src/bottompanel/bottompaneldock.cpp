#include "bottompaneldock.h"

#include <QTabWidget>

#include "mainwindow.h"

BottomPanelDock::BottomPanelDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle(tr("BottomPanel"));
    mpTabWgt = new QTabWidget;

    // issues
    QWidget *pIssuesTab = new QWidget;
    mpTabWgt->addTab(pIssuesTab, tr("Issues"));

    // compilation information
    QWidget *pCompileInfo = new QWidget;
    mpTabWgt->addTab(pCompileInfo, tr("Compile Output"));

    // debugging
    QWidget *pDebugConsole = new QWidget;
    mpTabWgt->addTab(pDebugConsole, tr("Debugger Console"));

    // version control
    QWidget *pVersionsCtrl = new QWidget;
    mpTabWgt->addTab(pVersionsCtrl, tr("Terminal"));

    setWidget(mpTabWgt);
}
