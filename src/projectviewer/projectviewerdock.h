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
    explicit ProjectViewerDock(QStringList filters,
                               QWidget *pParent = nullptr);
    void setFilters(QStringList filters);
    void setDir(QDir curDir);

public slots:
    void onOpenFileFromProjectViewer(QString fileName);

signals:
    void openFileFromProjectViewer(QString);
};

#endif // PROJECTVIEWERDOCK_H
