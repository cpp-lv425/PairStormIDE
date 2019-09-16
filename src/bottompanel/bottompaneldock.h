#ifndef BOTTOMPANELDOCK_H
#define BOTTOMPANELDOCK_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QTabWidget;
class ConsoleWindow;
class CompileWindow;
QT_END_NAMESPACE

class BottomPanelDock: public QDockWidget
{
    Q_OBJECT

    QTabWidget *mpTabWgt;
public:
    explicit BottomPanelDock(QWidget *pParent = nullptr);
    ConsoleWindow *getPTerminalConsole() const;
    void setCompileAsCurrentTab();

signals:
    void projectPathWasChanged(QString);
    void programIsReadyToCompile();
    void programIsReadyToRun();
public slots:
    void reSendProjectPathChanged(QString);
    void reSendProgramIsReadyToCompile();
    void reSendProgramIsReadyToRun();
private:
    QWidget *mpIssuesTab;
    CompileWindow *mpCompileInfo;
    QWidget *mpDebugConsole;
    ConsoleWindow *mpTerminalConsole;
};

#endif // BOTTOMPANELDOCK_H
