#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>
#include <QMdiArea>
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
    MDIArea *mBrowseArea;
    ~Browser();
public slots:
    void newTab(const QString &keyword);
    void emptyDocumentationTab();
private:

    ConnectionManager *mConnectionManager;
    DocumentationEngine *mDocumentationEngine;
    QVector<QString> mTempHTMLFiles;
};

#endif // BROWSER_H
