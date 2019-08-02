#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class MDIArea;
class ConnectionManager;
class DocumentationEngine;
QT_END_NAMESPACE

class Browser : public QMainWindow
{
    Q_OBJECT
public:
    explicit Browser(QWidget *parent = nullptr);
    ~Browser();
public slots:
    void newTab(const QString &keyword);
    void emptyDocumentationTab();
private:
    MDIArea *mBrowseArea;
    ConnectionManager *mConnectionManager;
    DocumentationEngine *mDocumentationEngine;
    QVector<QString> mTempHTMLFiles;
};

#endif // BROWSER_H
