#ifndef MDIAREA_H
#define MDIAREA_H

#include <QObject>

class QMdiSubWindow;
class CodeEditor;
class QSplitter;
class QMdiArea;

class DocumentManager: public QObject
{
    Q_OBJECT

    QSplitter *mpSplitter;
    QVector<QMdiArea*> mDocAreas;
    CodeEditor *mpPrevEditorInFocus;

public:
    explicit DocumentManager();
    void splitWindow();
    QSplitter* getSplitter();
    void openDocument(const QString &fileName, bool load = false);
    void loadFile(CodeEditor *newView, const QString &fileName);

public slots:
    void onSplit(Qt::Orientation orientation);
    void onFocusChanged(QWidget *old, QWidget *now);
    void onCloseDocument(CodeEditor *doc);

private:
    QMdiArea* createMdiArea();
    QMdiArea* selectAreaForPlacement();
    QMdiSubWindow* openedDoc(const QString &fileName);
    QMdiArea* areaInFocus();
    QMdiArea* getArea(CodeEditor *doc);
};

#endif // MDIAREA_H
