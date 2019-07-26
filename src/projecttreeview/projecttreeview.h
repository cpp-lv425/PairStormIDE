#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>

class ProjectViewerModel;

class ProjectTreeView : public QTreeView
{
public:
    ProjectTreeView(ProjectViewerModel *mPrModel,QWidget * parent = nullptr);
    // QWidget interface
protected:
    //virtual void keyPressEvent(QKeyEvent *event) override;
    //virtual void mouseClickEvent(QMouseEvent * event) override;
private:
    ProjectViewerModel *mPrModel;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // PROJECTTREEVIEW_H
