#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>

QT_BEGIN_NAMESPACE
class QDir;
class ProjectViewerModel;
QT_END_NAMESPACE

class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    ProjectTreeView(QWidget * parent = nullptr);
    ProjectTreeView(ProjectViewerModel *model,QWidget * parent = nullptr);
    void setProjectModel(ProjectViewerModel *prModel);
    void setFilters(const QStringList &filters);
    void setDirectory(const QDir &directory);

signals:
    void codeFileSelected(QString);

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    ProjectViewerModel *mProjectModel;
};

#endif // PROJECTTREEVIEW_H
