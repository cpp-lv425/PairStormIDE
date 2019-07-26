#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>

class ProjectViewerModel;

class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    ProjectTreeView(ProjectViewerModel *mPrModel,QWidget * parent = nullptr);
signals:
    void codeFileSelected(QString);
    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    ProjectViewerModel *mPrModel;
};

#endif // PROJECTTREEVIEW_H
