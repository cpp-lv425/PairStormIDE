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
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
signals:
    void codeFileSelected(QString filePath);
private:
    ProjectViewerModel *mPrModel;
};

#endif // PROJECTTREEVIEW_H
