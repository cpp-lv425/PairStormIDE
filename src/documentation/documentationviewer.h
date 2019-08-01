#include <QMainWindow>
#include <QObject>
#include <QWebEngineView>
#include <QLineEdit>
#include <QProgressBar>
#include <QStackedWidget>
#include <QAction>
#include <QKeyEvent>
#include <QToolBar>
#include <QWebEnginePage>

class DocumentationViewer : public QMainWindow {

    Q_OBJECT

public:
    DocumentationViewer(QWidget *parent = nullptr);

private:
    QLineEdit *mUrlEdit;
    QStatusBar *mStatusBar;

    QToolBar *mBottomToolBar;
    QWebEngineView *mWebView;

    QProgressBar *mProgressBar;
    QStackedWidget *mStackedWidget;

    QWebEnginePage *mCurrentPage;

    QAction *mPrevPageAction;
    QAction *mNextPageAction;
    QAction *mStopLoadAction;
    QAction *mRefreshPageAction;

    void setupUI();

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
