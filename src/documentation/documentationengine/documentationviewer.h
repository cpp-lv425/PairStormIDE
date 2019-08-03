#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QProgressBar;
class QWebEngineView;
class QStackedWidget;
class QWebEnginePage;
class ConnectionManager;
class DocumentationEngine;
QT_END_NAMESPACE
class DocumentationViewer : public QMainWindow
{
    Q_OBJECT

public:
    DocumentationViewer(QWidget *parent = nullptr);

    QWebEngineView *webView() const;

    void loadReferenceDocumentation();
    void loadReferenceDocumentation(const QString &keyword);

private:
    QLineEdit *mUrlEdit;

    QToolBar *mBottomToolBar;
    QWebEngineView *mWebView;

    QProgressBar *mProgressBar;
    QStackedWidget *mStackedWidget;

    QWebEnginePage *mCurrentPage;

    QAction *mPrevPageAction;
    QAction *mNextPageAction;
    QAction *mStopLoadAction;
    QAction *mRefreshPageAction;

    ConnectionManager *mConnectionManager;
    DocumentationEngine *mDocumentationEngine;

private slots:
    void onHistoryTriggered();
    void onHelpTrigerred();
    void onExitTrigerred();

    void updateUrlBar(const QUrl &url);
    void urlRequested();

    void titleChange(const QString &title);

    void loadStarted();
    void loadFinished(bool ok);

    void iconChanged();
    void linkHovered(const QString &url);
};
