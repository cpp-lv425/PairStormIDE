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
    mpIssuesTab = new QWidget;
    mpTabWgt->addTab(mpIssuesTab, tr("Issues"));

    // compilation information
    mpCompileInfo = new CompileWindow;
    mpTabWgt->addTab(mpCompileInfo, tr("Compile Output"));
    connect(mpCompileInfo, &CompileWindow::programIsReadyToCompile,
            this, &BottomPanelDock::reSendProgramIsReadyToCompile);

    // debugging
    mpDebugConsole = new QWidget;
    mpTabWgt->addTab(mpDebugConsole, tr("Debugger Console"));

    // version control and console
    mpTerminalConsole = new ConsoleWindow;
    mpTabWgt->addTab(mpTerminalConsole, tr("Terminal"));

    connect(this, &BottomPanelDock::projectPathWasChanged,
            mpTerminalConsole, &ConsoleWindow::setProjectPath);

    connect(mpTerminalConsole, &ConsoleWindow::errorsAreOccuredAfterCompilation,
            mpCompileInfo, &CompileWindow::setCompileErrorsOutput);

    connect(mpTerminalConsole, &ConsoleWindow::messageAboutNoErrorsAfterCompilation,
            mpCompileInfo, &CompileWindow::setCompileSuccessOutput);

    setWidget(mpTabWgt);
}

void BottomPanelDock::reSendProjectPathChanged(QString path)
{
    emit projectPathWasChanged(path);
}

void BottomPanelDock::reSendProgramIsReadyToCompile()
{
    emit programIsReadyToCompile();
    mpCompileInfo->clearCompileOutputView();
}

void BottomPanelDock::reSendProgramIsReadyToRun()
{
    mpTerminalConsole->runExecutableFile();
}

ConsoleWindow* BottomPanelDock::getPTerminalConsole() const
{
    return mpTerminalConsole;
}

void BottomPanelDock::setCompileAsCurrentTab()
{
    mpTabWgt->setCurrentWidget(mpCompileInfo);
}
