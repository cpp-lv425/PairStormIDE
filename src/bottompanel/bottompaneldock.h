#ifndef BOTTOMPANELDOCK_H
#define BOTTOMPANELDOCK_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QTabWidget;
class ConsoleWindow;
QT_END_NAMESPACE

class BottomPanelDock: public QDockWidget
{
    Q_OBJECT

    QTabWidget *mpTabWgt;
public:
    explicit BottomPanelDock(QWidget *pParent = nullptr);
signals:
    void projectPathWasChanged(QString);
public slots:
    void reSendProjectPathChanged(QString);
private:
    QWidget *pIssuesTab;
    QWidget *pCompileInfo;
    QWidget *pDebugConsole;
    ConsoleWindow *pTerminalConsole;
};

#endif // BOTTOMPANELDOCK_H
