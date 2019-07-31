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
#include "newfilewizard.h"
#include "filemanager.h"
#include "codeeditor.h"
#include "mdiarea.h"
#include "storeconf.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    {
        StoreConf conf(this);
    }

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

QStringList MainWindow::getFileExtensions() const
{
    return QStringList() << ".c" << ".cpp" << ".h" << ".hpp" << ".txt" << ".json";
}

void MainWindow::showStartPage()
{
    StartPage startPage(this);
    connect(&startPage, &StartPage::onNewBtnPressed, this, &MainWindow::onNewFileTriggered);
    connect(&startPage, &StartPage::onOpenBtnPressed, this, &MainWindow::onOpenFileTriggered);
    connect(&startPage, &StartPage::onOpenDirPressed, this, &MainWindow::onOpenFolderTriggered);
    connect(&startPage, &StartPage::onReferenceBtnPressed, this, &MainWindow::onReferenceTriggered);
    connect(&startPage, &StartPage::onSettingsBtnPressed, this, &MainWindow::onSettingsTriggered);
    startPage.showStartPage();
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
    fileMenu->addAction("Save &As...", this, &MainWindow::onSaveFileAsTriggered);
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

void MainWindow::saveDocument(CodeEditor *pDoc, QString fileName)
{
    qDebug() << "saving";
    try
    {
        // writing to file
        FileManager().writeToFile(fileName, pDoc->toPlainText());
    } catch (const QException&)
    {
        QMessageBox::warning(this, "Error", "Unable to open file for saving");
    }
}

void MainWindow::openDoc(QString fileName)
{
    QString readResult;

    try
    {
        readResult = FileManager().readFromFile(fileName);
    } catch (const QException&)
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Unable to open specified file."));
        return;
    }

    // creating new doc & passing file content to it
    CodeEditor *newDoc = createNewDoc();
    newDoc->setFileName(fileName);
    int position = fileName.lastIndexOf(QChar{'/'});
    newDoc->setWindowTitle(fileName.mid(position + 1));
    newDoc->setPlainText(readResult);
    newDoc->show();
}

void MainWindow::onNewFileTriggered()
{
    QStringList fileExtensions = getFileExtensions();
    NewFileDialog newFileDialog(fileExtensions, this);

    // new file dialog is called
    // name of newly created file is received
    QString newFileName = newFileDialog.start();

    if(newFileName.isEmpty())
        return;

    // new doc is created & shown
    CodeEditor *newDoc = createNewDoc();
    int position = newFileName.lastIndexOf(QChar{'/'});
    newDoc->setFileName(newFileName);
    newDoc->setWindowTitle(newFileName.mid(position + 1));
    newDoc->show();
}

void MainWindow::onOpenFileTriggered()
{
    QString fileName = QFileDialog::getOpenFileName
            (
                this,
                "Open File",
                QDir::currentPath(),
                "C++/C files (*.h *.hpp *.cpp *.c) ;; Text Files (*.txt) ;; JSON Files (*.json)"
                );
    openDoc(fileName);
}

void MainWindow::onOpenFolderTriggered()
{
    QString dirName = QFileDialog::getExistingDirectory(this, "Open Directory", QDir::currentPath());
    mpProjectViewerDock->setDir(dirName);
}

void MainWindow::onOpenStartPage()
{
    showStartPage();
}

void MainWindow::onSaveFileTriggered()
{
    // if there are no opened docs
    if(!mpDocsArea || !mpDocsArea->currentSubWindow())
    {
        QMessageBox::information(this, "Save", "There are no opened documents to save.");
        return;
    }

    auto curDoc = qobject_cast<CodeEditor*>
            (mpDocsArea->currentSubWindow()->widget());

    // if ptr to current document is not valid
    if(!curDoc)
        return;

    // if doc wasn't modified yet
    if(!curDoc->document()->isModified())
        return;

    // saving doc
    saveDocument(curDoc, curDoc->getFileName());
}

void MainWindow::onSaveFileAsTriggered()
{
    // if there are no opened docs
    if(!mpDocsArea || !mpDocsArea->currentSubWindow())
    {
        QMessageBox::information(this, "Save", "There are no opened documents to save.");
        return;
    }

    auto curDoc = qobject_cast<CodeEditor*>
            (mpDocsArea->currentSubWindow()->widget());

    // if ptr to current document is not valid
    if(!curDoc)
        return;

    QString extension;

    QString fileName = QFileDialog::getSaveFileName
            (
                this,
                "Save As",
                QDir::currentPath() + "/Unnamed",
                "*.h ;; *.hpp ;; *.cpp ;; *.c ;; *.txt ;; *.json",
                &extension
                );

    int position = fileName.indexOf(QChar{'.'});
    fileName += extension.mid(position + 1);

    // saving doc
    saveDocument(curDoc, fileName);
}

void MainWindow::onSaveAllFilesTriggered()
{
    if(!mpDocsArea)
        return;

    // getting all docs
    auto docsList = mpDocsArea->subWindowList();    

    // if there are no docs
    if(docsList.empty())
    {
        QMessageBox::information(this, "Save", "There are no opened documents to save.");
        return;
    }

    // if doc is modified then it is saved
    for (int i = 0; i < docsList.size(); ++i)
    {
        auto curDoc = qobject_cast<CodeEditor*>(docsList[i]->widget());
        if(!curDoc)
            qDebug() << "invalid ptr";
        if(curDoc && curDoc->document()->isModified())
        {
            qDebug() << curDoc->getFileName();
            saveDocument(curDoc, curDoc->getFileName());
        }
    }
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

void MainWindow::onOpenFileFromProjectViewer(QString fileName)
{
    openDoc(fileName);
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
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
}

// restore state and geometry of mainwindow's toolbars and dockwidgets from last session
void MainWindow::restoreMainWindowState()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
}
