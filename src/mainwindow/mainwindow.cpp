#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPlainTextEdit> // temporarily included
#include <QMdiSubWindow>
#include <QStyleFactory>
#include <QMessageBox>
#include <QFileDialog>
#include <QException> // temporarily included
#include <QDebug> // temporarily included
#include <QFile>
#include <QSettings>

#include "projectviewerdock.h"
#include "bottompaneldock.h"
#include "chatwindowdock.h"
#include "filemanager.h"
#include "codeeditor.h"
#include "mdiarea.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // when first started main window is maximized
    setWindowState(Qt::WindowMaximized);

    // set Fusion style globally - TEMP SOLUTION
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    setupMainMenu();

    // create instance of Project Viewer
    mpProjectViewerDock = new ProjectViewerDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, mpProjectViewerDock);
    mpProjectViewerDock->setObjectName("mpProjectViewerDock");

    // create instance of Chat Window
    mpChatWindowDock = new ChatWindowDock(this);
    mpChatWindowDock->setObjectName("mpChatWindowDock");
    addDockWidget(Qt::RightDockWidgetArea, mpChatWindowDock);

    // create instance of MDIArea
    mpDocsArea = new MDIArea(this);   

    // create instance of Bottom Panel
    mpBottomPanelDock = new BottomPanelDock(this);
    mpBottomPanelDock->setObjectName("mpBottomPanelDock");

    setCentralWidget(mpDocsArea);

    restoreMainWindowState();
}

void MainWindow::setupMainMenu()
{
    // file menu
    QMenu *fileMenu = new QMenu("&File");
    // working with files
    fileMenu->addAction("&New file", this, &MainWindow::onNewFileTriggered, Qt::CTRL + Qt::Key_N);
    fileMenu->addAction("&Open file...", this, &MainWindow::onOpenFileTriggered, Qt::CTRL + Qt::Key_O);
    fileMenu->addAction("Open &folder...", this, &MainWindow::onOpenFolderTriggered);
    fileMenu->addSeparator();

    // opening start page
    fileMenu->addAction("Start &Page...", this, &MainWindow::onOpenStartPage);
    fileMenu->addSeparator();

    // saving files
    fileMenu->addAction("&Save...", this, &MainWindow::onSaveFileTriggered, Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("Save &As...", this, &MainWindow::onSaveFileAsTriggered, Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("Save A&ll...", this, &MainWindow::onSaveAllFilesTriggered, Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    fileMenu->addSeparator();

    // closing docs & exiting the program
    fileMenu->addAction("&Close file", this, &MainWindow::onCloseFileTriggered, Qt::CTRL + Qt::Key_W);
    fileMenu->addAction("&Exit", this, &MainWindow::onExitTriggered, Qt::ALT + Qt::Key_F4);

    // edit menu
    QMenu *editMenu = new QMenu("&Edit");

    // undo / redo
    editMenu->addAction("&Undo", this, &MainWindow::onUndoTriggered, Qt::CTRL + Qt::Key_Z);
    editMenu->addAction("&Redo", this, &MainWindow::onRedoTriggered, Qt::CTRL + Qt::Key_Y);
    editMenu->addSeparator();

    // working with clipboard
    editMenu->addAction("Cu&t", this, &MainWindow::onCutTriggered, Qt::CTRL + Qt::Key_X);
    editMenu->addAction("&Copy", this, &MainWindow::onCopyTriggered, Qt::CTRL + Qt::Key_C);
    editMenu->addAction("&Paste", this, &MainWindow::onPasteTriggered, Qt::CTRL + Qt::Key_V);
    editMenu->addAction("Select &All", this, &MainWindow::onSelectAllTriggered, Qt::CTRL + Qt::Key_A);
    editMenu->addSeparator();

    // find patterns in docs
    editMenu->addAction("&Find/Replace...", this, &MainWindow::onFindTriggered, Qt::CTRL + Qt::Key_F);

    // view menu
    QMenu *viewMenu = new QMenu("&View");
    viewMenu->addAction("&Full screen", this, &MainWindow::onFullScreenTriggered, Qt::CTRL + Qt::SHIFT + Qt::Key_F11);
    QMenu *scaleSubMenu = new QMenu("&Scale");
    viewMenu->addMenu(scaleSubMenu);

    // tools menu
    QMenu *toolsMenu = new QMenu("&Tools");

    // opening refactoring toolbar
    toolsMenu->addAction("&Refactor...", this, &MainWindow::onRefactorTriggered);
    toolsMenu->addSeparator();

    // opening chat window
    toolsMenu->addAction("&Connect...", this, &MainWindow::onConnectTriggered);
    toolsMenu->addSeparator();

    // buidling solution
    QMenu *buildSubMenu = new QMenu("&Build");
    toolsMenu->addMenu(buildSubMenu);

    // debugging program
    QMenu *debugSubMenu = new QMenu("&Debug");
    toolsMenu->addMenu(debugSubMenu);

    // git
    QMenu *gitSubMenu = new QMenu("&Git");
    toolsMenu->addMenu(gitSubMenu);
    toolsMenu->addSeparator();

    // opening settings window
    toolsMenu->addAction("&Settings...", this, &MainWindow::onSettingsTriggered);

    // help menu
    QMenu *helpMenu = new QMenu("&Help");

    // info about program
    helpMenu->addAction("&About...", this, &MainWindow::onAboutTriggered);
    helpMenu->addSeparator();

    // opening reference window
    helpMenu->addAction("&Reference Assistant...", this, &MainWindow::onReferenceTriggered, Qt::CTRL + Qt::Key_F1);

    // user guide
    helpMenu->addAction("User &Guide...", this, &MainWindow::onUserGuideTriggered, Qt::Key_F1);
    helpMenu->addSeparator();

    // checking updates
    helpMenu->addAction("Check for &Updates...", this, &MainWindow::onCheckUpdatesTriggered);

    // adding all menus to MainMenuBar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::onNewFileTriggered()
{
    QString fileName = QFileDialog::getSaveFileName
            (this, tr("Enter new filename"), QDir::homePath());
    try
    {
        FileManager().createFile(fileName);
        CodeEditor *newDoc = createNewDoc();
        newDoc->setWindowTitle(fileName);
        newDoc->show();
    } catch (const QException& e)
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Unable to create new file."));
    }
}

void MainWindow::onOpenFileTriggered()
{
    QString fileName = QFileDialog::getOpenFileName();
    QString readResult;
    try
    {
        readResult = FileManager().readFromFile(fileName);
    } catch (const QException& e)
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Unable to open specified file."));
        return;
    }    

    CodeEditor *newDoc = createNewDoc();
    newDoc->setPlainText(readResult);
    newDoc->show();
}

void MainWindow::onOpenFolderTriggered()
{
//    QString dirName = QFileDialog::getExistingDirectory
//            (this, "Select directory", QDir::homePath());
//    QDir selectedDir(dirName);

//    QStringList filesList = selectedDir.entryList(QDir::Files);

//    for (const QString& file : filesList)
//    {
//        QString readResult;
//        try
//        {
//            readResult = FileManager().readFromFile(dirName + '/' + file);
//        } catch (const QException& e)
//        {
//            QMessageBox::warning(this, tr("Error"),
//                                 tr("Unable to open file from specified directory"));
//            return;
//        }

//        QWidget *newDoc = createNewDoc();
//        newDoc->setWindowTitle(file);

//        QPlainTextEdit *pTextEdit = new QPlainTextEdit;
//        pTextEdit->setPlainText(readResult);

//        pLayout->addWidget(pTextEdit);
//        newDoc->setLayout(pLayout);
//        newDoc->show();
//    }
}

void MainWindow::onOpenStartPage()
{
    qDebug() << "open start page";
}

void MainWindow::onSaveFileTriggered()
{
    QMdiSubWindow *pCurrentSubWdw =
            mpDocsArea->currentSubWindow();

    // get filename
    QString filename;
    // write to file
    //writeToFile(filename, pCurrentDoc->toPlainText());
}

void MainWindow::onSaveFileAsTriggered()
{
    qDebug() << "save as";
}

void MainWindow::onSaveAllFilesTriggered()
{
    qDebug() << "save all";
}

void MainWindow::onCloseFileTriggered()
{
    qDebug() << "close file";
}

void MainWindow::onExitTriggered()
{
    QApplication::closeAllWindows();
}

void MainWindow::onUndoTriggered()
{
    qDebug() << "undo";
}

void MainWindow::onRedoTriggered()
{
    qDebug() << "redo";
}

void MainWindow::onCutTriggered()
{
    qDebug() << "cut";
}

void MainWindow::onCopyTriggered()
{
    qDebug() << "copy";
}

void MainWindow::onPasteTriggered()
{
    qDebug() << "paste";
}

void MainWindow::onSelectAllTriggered()
{
    qDebug() << "select all";
}

void MainWindow::onFindTriggered()
{
    qDebug() << "find";
}

void MainWindow::onFullScreenTriggered()
{
    qDebug() << "full screen";
}

void MainWindow::onRefactorTriggered()
{
    qDebug() << "refactor";
}

void MainWindow::onConnectTriggered()
{
    qDebug() << "connect";
}

void MainWindow::onSettingsTriggered()
{
    qDebug() << "settings";
}

void MainWindow::onAboutTriggered()
{
    qDebug() << "about";
}

void MainWindow::onReferenceTriggered()
{
    qDebug() << "reference";
}

void MainWindow::onUserGuideTriggered()
{
    qDebug() << "user guide";
}

void MainWindow::onCheckUpdatesTriggered()
{
    qDebug() << "check updates";
}

CodeEditor* MainWindow::createNewDoc()
{
    CodeEditor *newDoc = new CodeEditor;
    mpDocsArea->addSubWindow(newDoc);
    newDoc->setAttribute(Qt::WA_DeleteOnClose);

    return newDoc;
}

MainWindow::~MainWindow()
{
    saveMainWindowState();
    delete ui;
}

// save to configuration file current state and geometry of mainwindow's toolbars and dockwidgets
void MainWindow::saveMainWindowState()
{
    QSettings settings("425", "PairStorm");
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
}

// restore state and geometry of mainwindow's toolbars and dockwidgets from last session
void MainWindow::restoreMainWindowState()
{
    QSettings settings("425", "PairStorm");
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
}
