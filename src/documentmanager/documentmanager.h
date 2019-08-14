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
    QSplitter* getSplitter();
    void openDocument(const QString &fileName, bool load = false);
    bool saveDocument();
    bool saveAllDocuments();
    void saveDocumentAs(CodeEditor *currentDocument, const QString &fileName);
    CodeEditor* getCurrentDocument();
    void closeCurrentDocument();
    QVector<CodeEditor*> getChangedDocuments();

public slots:
    void onSplit(Qt::Orientation orientation);
    void onFocusChanged(QWidget *old, QWidget *now);
    void onCloseDocument(CodeEditor *doc);

private:
    void splitWindow();
    void loadFile(CodeEditor *newView, const QString &fileName);

    QMdiArea* createMdiArea();
    QMdiArea* selectAreaForPlacement();
    QMdiSubWindow* openedDoc(const QString &fileName);
    QMdiArea* lastAreaInFocus();
    QMdiArea* areaInFocus();
    QMdiArea* getArea(CodeEditor *doc);
    bool saveDocument(CodeEditor* doc);
    void saveDocument(const QString &fileName, const QString &fileContent);

};

#endif // MDIAREA_H
