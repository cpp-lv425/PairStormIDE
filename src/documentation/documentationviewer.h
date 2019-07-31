#ifndef DOCUMENTATIONVIEWER_H
#define DOCUMENTATIONVIEWER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDir;
class QListView;
class QWebEngineView;
class ConnectionManager;
class DocumentationWebEngine;
QT_END_NAMESPACE

class DocumentationViewer : public QDialog
{
    Q_OBJECT
public:
    explicit DocumentationViewer(QWidget *parent = nullptr);
    ~DocumentationViewer() = default;
    void setUrl(const QUrl &url);


private:
    QWebEngineView *mWebView;
    QListView *mDocumentsView;
    ConnectionManager *mManager;
    DocumentationWebEngine *mDocumentationEngine;
};

#endif // DOCUMENTATIONVIEWER_H
