#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

QT_BEGIN_NAMESPACE
class LocalConnectorInterface;
class SettingsConfigurator;
class PaletteConfigurator;
class ProjectViewerDock;
class QFileSystemModel;
class NewProjectDialog;
class BottomPanelDock;
class DocumentManager;
class QListWidgetItem;
class ChatWindowDock;
class QMdiSubWindow;
class CodeEditor;
class Browser;
class Connection;
class FileDb;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QStringList getFileExtensions()const;
    void showStartPage();
    ~MainWindow();

    bool mIsFinished;
private:
    LocalConnectorInterface *mplocalConnector;
    Ui::MainWindow *ui;
    ProjectViewerDock *mpProjectViewerDock;
    ChatWindowDock *mpChatWindowDock;
    BottomPanelDock *mpBottomPanelDock;
    QScopedPointer<DocumentManager> mpDocumentManager;    
    Browser *mDocumentationBrowser;
    QScopedPointer<PaletteConfigurator> mpPaletteConfigurator;
    Connection *db;
    FileDb* dbFileManager;

    void setupMainMenu();    
    void openDocument(const QString &fileName);
    void createProjectViewer();
    void createChatWindow();
    void createButtomPanel();

    void saveMainWindowState();
    void restoreMainWindowState();
    void setInitialAppStyle();

    // methods for dynamic change of IDE settings
    void setAppStyle(const QString &style);
    void setDocumentFontFamily(const QString &fontFamily);
    void setDocumentFontSize(const QString &fontSize);

    void databaseConnect();
    void databaseDisconnect();
    void restoreDatabaseFile();
    void hideDatabaseFile();
    void setDatabaseFileName();
    void readWriteFileContent(QString fileToReadName, QString fileToWriteName);

private slots:
    // file menu actions
    void onNewProjectTriggered();
    void onNewFileTriggered();
    void onNewClassTriggered();
    void onOpenFileTriggered();
    void onOpenProjectTriggered();
    void onCloseProjectTriggered();
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
    void onSplitHorizontallyTriggered();
    void onSplitVerticallyTriggered();
    void onShowProjectViewerTriggered();
    void onShowChatWindowDockTriggered();
    void onShowBottomPanel();
    void onCombineAreas();
    void onCloseEmptyDocArea();   

    // tools menu
    void onRefactorTriggered();
    void onConnectTriggered();
    void onSettingsTriggered();

    // help menu
    void onAboutTriggered();
    void onReferenceTriggered();
    void onUserGuideTriggered();
    void onCheckUpdatesTriggered();
    void onReferenceFromEditor(const QString &keyword);

public slots:
    void onOpenFileFromProjectViewer(QString fileName);
    void onConnectionStatusChanged(bool status);
    void onSettingsChanged(std::map<QString, QString> newValues);
    void openCreatedClassFiles(QString headerFile, QString sourceFile);

private:
    friend class SettingsConfigurator;
    QString  databaseFileName;
    QString dirName;
    const char *projectDatabaseExtension = ".db";
    const char pathSeparator = '/';

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
