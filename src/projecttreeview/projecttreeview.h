#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>

class ProjectViewerModel;

class ProjectTreeView : public QTreeView
{
    // QWidget interface
protected:
    //virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent * event) override;
private:
    ProjectViewerModel *mModel;
};

#endif // PROJECTTREEVIEW_H
