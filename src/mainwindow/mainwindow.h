#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

QT_BEGIN_NAMESPACE
class LocalConnectorInterface;
class ProjectViewerDock;
class QFileSystemModel;
class BottomPanelDock;
class QListWidgetItem;
class ChatWindowDock;
class QMdiSubWindow;
class CodeEditor;
class MDIArea;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QStringList getFileExtensions()const;
    void showStartPage();
    ~MainWindow();

private:
    LocalConnectorInterface * mplocalConnector;
    Ui::MainWindow *ui;
    ProjectViewerDock *mpProjectViewerDock;
    ChatWindowDock *mpChatWindowDock;
    BottomPanelDock *mpBottomPanelDock;
    MDIArea *mpDocsArea;
    QString mCurrentUserName;
    void setupMainMenu();
    void saveDocument(CodeEditor *pDoc, const QString &fileName);
    void openDoc(QString fileName);
    bool isOpened(const QString &fileName)const;
    // returns true if any of docs has been modified
    bool isModified(QList<QMdiSubWindow*> &docsList);
    void saveAllModifiedDocuments(QList<QMdiSubWindow*> &docsList);
    void createProjectViewer();
    void createChatWindow();
    void createButtomPanel();
    CodeEditor* getCurrentDoc();

    void saveMainWindowState();
    void restoreMainWindowState();
    void setAppStyle();

private slots:
    // file menu actions
    void onNewFileTriggered();
    void onOpenFileTriggered();
    void onOpenFolderTriggered();
    void onOpenStartPage();
    void onSaveFileTriggered();
    void onSaveFileAsTriggered();
    void onSaveAllFilesTriggered();
    void onCloseFileTriggered();
    void onExitTriggered();

    // edit menu actions
    void onUndoTriggered();
    void onRedoTriggered();
    void onCutTriggered();
    void onCopyTriggered();
    void onPasteTriggered();
    void onSelectAllTriggered();
    void onFindTriggered();

    // view menu
    void onFullScreenTriggered();
    void onShowProjectViewerTriggered();
    void onShowChatWindowDockTriggered();
    void onShowBottomPanel();

    // tools menu
    void onRefactorTriggered();
    void onConnectTriggered();
    void onSettingsTriggered();

    // help menu
    void onAboutTriggered();
    void onReferenceTriggered();
    void onUserGuideTriggered();
    void onCheckUpdatesTriggered();

public slots:
    void onOpenFileFromProjectViewer(QString fileName);    
    void onCloseWindow(CodeEditor *curDoc);   
    void onConnectionStatusChanged(bool status);

private:
    // creates new doc in MDIArea
    CodeEditor* createNewDoc();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
