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
signals:
    void projectPathWasChanged(QString);
    void programIsReadyToCompile();
public slots:
    void reSendProjectPathChanged(QString);
    void reSendProgramIsReadyToCompile();
private:
    QWidget *pIssuesTab;
    CompileWindow *pCompileInfo;
    QWidget *pDebugConsole;
    ConsoleWindow *pTerminalConsole;
};

#endif // BOTTOMPANELDOCK_H
