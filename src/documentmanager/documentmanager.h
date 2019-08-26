#ifndef MDIAREA_H
#define MDIAREA_H

#include <functional>
#include <QObject>

class QMdiSubWindow;
class CodeEditor;
class QSplitter;
class QMdiArea;

class DocumentManager: public QObject
{
    Q_OBJECT

    // object responsible for laying out several doc area
    QSplitter *mpSplitter;
    QVector<QMdiArea*> mDocAreas;
    // pointer to previous document in focus
    // is used when document loses focus upon user input
    // (e.g. when Project Viewer item is clicked to open another document)
    CodeEditor *mpPrevEditorInFocus;
    QString currentProject;

public:
    explicit DocumentManager();
    QSplitter* getSplitter();
    void openProject(const QString &path);
    const QString& getCurrentProjectPath()const;
    void closeCurrentProject();
    void openDocument(const QString &fileName, bool load = false);
    bool saveDocument();
    bool saveAllDocuments();
    void saveDocumentAs(CodeEditor *currentDocument, const QString &fileName);
    CodeEditor* getCurrentDocument();
    void closeCurrentDocument();
    void closeAllDocumentsWithoutSaving();
    QVector<CodeEditor*> getChangedDocuments();
    void combineDocAreas();
    void closeEmptyDocArea();    

    // calls passed functor on every opened document
    void configureDocuments(std::function<void(DocumentManager*, CodeEditor*, const QString&)> functor,
                           const QString &newValue);
    void setStyle(CodeEditor *doc, const QString &styleName);
    void setFontFamily(CodeEditor *doc, const QString &fontFamily);
    void setFontSize(CodeEditor *doc, const QString &fontSize);

public slots:
    void onSplit(Qt::Orientation orientation);
    void onFocusChanged(QWidget *old, QWidget *now);
    void onCloseDocument(CodeEditor *doc);

private:
    void splitWindow();
    void loadFile(CodeEditor *newView, const QString &fileName);

    QMdiArea* createMdiArea();
    CodeEditor* createDoc(const QString &fileName);
    QMdiArea* selectAreaForPlacement();
    QMdiSubWindow* openedDoc(const QString &fileName);
    QMdiArea* lastAreaInFocus();
    QMdiArea* areaInFocus();
    QMdiArea* getArea(CodeEditor *doc);
    bool saveDocument(CodeEditor *doc);
    void saveDocument(const QString &fileName, const QString &fileContent);
    void setAllDocumentsNotModified();
};

#endif // MDIAREA_H
