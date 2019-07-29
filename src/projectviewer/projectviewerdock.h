#ifndef PROJECTVIEWERDOCK_H
#define PROJECTVIEWERDOCK_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class ProjectTreeView;
class QWidget;
QT_END_NAMESPACE

class ProjectViewerDock: public QDockWidget
{
    Q_OBJECT

    ProjectTreeView *pTreeViewer;
public:
    ProjectViewerDock(QWidget *pParent = nullptr);    
    ~ProjectViewerDock();
};

#endif // PROJECTVIEWERDOCK_H
