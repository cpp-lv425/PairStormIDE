#ifndef MDIAREA_H
#define MDIAREA_H

#include <QObject>

class TextDocumentHolder;
class QSplitter;
class QMdiArea;

class DocumentManager: public QObject
{
    Q_OBJECT

    QSplitter *mpSplitter;
    QVector<QMdiArea*> mDocAreas;
    QVector<QScopedPointer<TextDocumentHolder>> mDocuments;

public:
    explicit DocumentManager();
    void splitWindow();
    QSplitter *getSplitter();

private:
    QMdiArea *createMdiArea();
};

#endif // MDIAREA_H
