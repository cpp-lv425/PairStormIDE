#ifndef PROJECTVIEWERDOCK_H
#define PROJECTVIEWERDOCK_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QTreeView;
class QWidget;
QT_END_NAMESPACE

class ProjectViewerDock: public QDockWidget
{
    QTreeView *pTreeViewer;
public:
    ProjectViewerDock(QWidget *pParent = nullptr);
};

#endif // PROJECTVIEWERDOCK_H
