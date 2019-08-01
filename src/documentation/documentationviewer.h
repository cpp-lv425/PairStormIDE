#include <QMainWindow>
#include <QObject>
#include <QWebEngineView>
#include <QLineEdit>
#include <QProgressBar>
#include <QStackedWidget>
#include <QAction>
#include <QKeyEvent>
#include <QToolBar>

class DocumentationViewer : public QMainWindow {

    Q_OBJECT

public:
    DocumentationViewer(QWidget *parent = nullptr);

private:
    QLineEdit *mUrlEdit;
    QWebEngineView *mWebView;
    QProgressBar *mProgressBar;
    QStackedWidget *stacked_widget;
    QStatusBar *mStatusBar;
    QToolBar *mBottomToolBar;
    QLineEdit *phrase;

    QAction *mPrevPageAction;
    QAction *mNextPageAction;
    QAction *mStopLoadAction;
    QAction *mRefreshPageAction;


    void setupUI();

protected:
    void keyPressEvent(QKeyEvent *event);

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
    void linkHovered(const QString &url, const QString &title, const QString &content);
    //void find();

};
