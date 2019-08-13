#ifndef MDIAREA_H
#define MDIAREA_H

#include <QObject>

class TextDocumentHolder;
class CodeEditor;
class QSplitter;
class QMdiArea;

class DocumentManager: public QObject
{
    Q_OBJECT

    QSplitter *mpSplitter;
    QVector<QMdiArea*> mDocAreas;
    QVector<QSharedPointer<TextDocumentHolder>> mDocuments;

public:
    explicit DocumentManager();
    void splitWindow();
    QSplitter* getSplitter();
    void openDocument(const QString &fileName, bool load = false);
    void loadFile(CodeEditor *newView, const QString &fileName);

public slots:
    void onSplit(Qt::Orientation orientation);

private:
    QMdiArea* createMdiArea();
    QMdiArea* selectAreaForPlacement();
    TextDocumentHolder* openedDoc(const QString &fileName);
};

#endif // MDIAREA_H
