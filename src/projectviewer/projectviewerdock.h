#ifndef PROJECTVIEWERDOCK_H
#define PROJECTVIEWERDOCK_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class ProjectViewerModel;
class ProjectTreeView;
class QWidget;
class QDir;
QT_END_NAMESPACE

class ProjectViewerDock: public QDockWidget
{
    Q_OBJECT

    ProjectTreeView *mpTreeViewer;
    ProjectViewerModel *mpViewerModel;
public:
    ProjectViewerDock(QWidget *pParent = nullptr);

    void setFilters(QStringList filters);
    void setDir(QDir curDir);

};

#endif // PROJECTVIEWERDOCK_H
