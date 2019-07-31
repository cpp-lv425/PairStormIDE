#ifndef DOCUMENTATIONVIEWER_H
#define DOCUMENTATIONVIEWER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDir;
class QListView;
class QWebEngineView;
class ConnectionManager;
QT_END_NAMESPACE

class DocumentationViewer : public QDialog
{
    Q_OBJECT
public:
    explicit DocumentationViewer(QWidget *parent = nullptr);
    ~DocumentationViewer();
    void setUrl(const QUrl &url);
private:
    QWebEngineView *mWebView;
    QListView *mDocumentsView;
    ConnectionManager *mManager;
};

#endif // DOCUMENTATIONVIEWER_H
