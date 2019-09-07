#include "bottompaneldock.h"
#include <QTabWidget>
#include "mainwindow.h"
#include "consolewindow/consolewindow.h"
#include <QDebug>

BottomPanelDock::BottomPanelDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle(tr("BottomPanel"));
    mpTabWgt = new QTabWidget;

    // issues
    pIssuesTab = new QWidget;
    mpTabWgt->addTab(pIssuesTab, tr("Issues"));

    // compilation information
    pCompileInfo = new QWidget;
    mpTabWgt->addTab(pCompileInfo, tr("Compile Output"));

    // debugging
    pDebugConsole = new QWidget;
    mpTabWgt->addTab(pDebugConsole, tr("Debugger Console"));

    // version control
    pTerminalConsole = new ConsoleWindow;
    mpTabWgt->addTab(pTerminalConsole, tr("Terminal"));

    connect(this, &BottomPanelDock::projectPathWasChanged,
            pTerminalConsole, &ConsoleWindow::setProjectPath);


    setWidget(mpTabWgt);
}

void BottomPanelDock::reSendProjectPathChanged(QString path)
{
    emit projectPathWasChanged(path);
}
