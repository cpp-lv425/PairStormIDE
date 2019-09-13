#include "bottompaneldock.h"
#include <QTabWidget>
#include "mainwindow.h"
#include "consolewindow/consolewindow.h"
#include "compilewindow.h"

BottomPanelDock::BottomPanelDock(QWidget *pParent): QDockWidget (pParent)
{
    setWindowTitle(tr("BottomPanel"));
    mpTabWgt = new QTabWidget;

    // issues
    pIssuesTab = new QWidget;
    mpTabWgt->addTab(pIssuesTab, tr("Issues"));

    // compilation information
    pCompileInfo = new CompileWindow;
    mpTabWgt->addTab(pCompileInfo, tr("Compile Output"));
    connect(pCompileInfo, &CompileWindow::programIsReadyToCompile,
            this, &BottomPanelDock::reSendProgramIsReadyToCompile);

    // debugging
    pDebugConsole = new QWidget;
    mpTabWgt->addTab(pDebugConsole, tr("Debugger Console"));

    // version control and console
    pTerminalConsole = new ConsoleWindow;
    mpTabWgt->addTab(pTerminalConsole, tr("Terminal"));

    connect(this, &BottomPanelDock::projectPathWasChanged,
            pTerminalConsole, &ConsoleWindow::setProjectPath);

    connect(pTerminalConsole, &ConsoleWindow::errorsAreOccuredAfterCompilation,
            pCompileInfo, &CompileWindow::setCompileOutput);


    setWidget(mpTabWgt);
}

void BottomPanelDock::reSendProjectPathChanged(QString path)
{
    emit projectPathWasChanged(path);
}

void BottomPanelDock::reSendProgramIsReadyToCompile()
{
    emit programIsReadyToCompile();
    pCompileInfo->clearCompileOutputView();
}

void BottomPanelDock::reSendProgramIsReadyToRun()
{
    emit programIsReadyToRun();
}

ConsoleWindow* BottomPanelDock::getPTerminalConsole() const
{
    return pTerminalConsole;
}

void BottomPanelDock::setCompileAsCurrentTab()
{
    mpTabWgt->setCurrentWidget(pCompileInfo);
}
