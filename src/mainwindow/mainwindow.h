#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

QT_BEGIN_NAMESPACE
class QDockWidget;
class QFileSystemModel;
class QMdiArea;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDockWidget *mpExplorerDock;
    QDockWidget *mpChatWindowDock;
    QDockWidget *mpBottomPanelDock;
    QMdiArea *mpDocsArea;

    void setMainMenu();

private slots:
    // file menu actions
    void newFileTriggered();
    void openFileTriggered();
    void openFolderTriggered();
    void openStartPage();
    void saveFileTriggered();
    void saveFileAsTriggered();
    void saveAllFilesTriggered();
    void closeFileTriggered();

    // edit menu actions
    void undoTriggered();
    void redoTriggered();
    void cutTriggered();
    void copyTriggered();
    void pasteTriggered();
    void selectAllTriggered();
    void findTriggered();

    // view menu
    void fullScreenTriggered();

    // tools menu
    void refactorTriggered();
    void connectTriggered();
    void settingsTriggered();

    // help menu
    void aboutTriggered();
    void referenceTriggered();
    void userGuideTriggered();
    void checkUpdatesTriggered();
};

#endif // MAINWINDOW_H
