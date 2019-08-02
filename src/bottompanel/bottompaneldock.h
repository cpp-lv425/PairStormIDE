#ifndef BOTTOMPANELDOCK_H
#define BOTTOMPANELDOCK_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QTabWidget;
QT_END_NAMESPACE

class BottomPanelDock: public QDockWidget
{
    Q_OBJECT

    QTabWidget *mpTabWgt;
public:
    explicit BottomPanelDock(QWidget *pParent = nullptr);
};

#endif // BOTTOMPANELDOCK_H
