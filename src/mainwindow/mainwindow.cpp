#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidgetItem>
#include <QMdiSubWindow>
#include <QStyleFactory>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QStyle>
#include <QDebug> // temporarily included
#include <QFile>

#include "projectviewerdock.h"
#include "bottompaneldock.h"
#include "chatwindowdock.h"
#include "newfilewizard.h"
#include "logindialog.h"
#include "filemanager.h"
#include "codeeditor.h"
#include "storeconf.h"
#include "startpage.h"
#include "mdiarea.h"
#include "utils.h"

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

    // set icon
    setWindowIcon(QIcon(":/img/app_logo.jpg"));

    // sets style globally
    setAppStyle();

    setupMainMenu();

    // create instance of MDIArea
    mpDocsArea = new MDIArea(this);
    setCentralWidget(mpDocsArea);

    // create instance of Project Viewer
    createProjectViewer();

    // create instance of Chat Window
    createChatWindow();

    // create instance of Bottom Panel
    createButtomPanel();

    restoreMainWindowState();
}

QStringList MainWindow::getFileExtensions() const
{
    QSettings settings;
    return QStringList {settings.value("cppExtentionsList").toStringList()};
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
    // main tool bar
    QToolBar *pToolbar = new QToolBar("Main Tool Bar");
    pToolbar->setObjectName("pToolbar");

    // working with files
    QAction *pNewFileAction = fileMenu->addAction("&New file", this, &MainWindow::onNewFileTriggered, Qt::CTRL + Qt::Key_N);
    pNewFileAction->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    pToolbar->addAction(pNewFileAction);

    QAction *pOpenFileAction = fileMenu->addAction("&Open file...", this, &MainWindow::onOpenFileTriggered, Qt::CTRL + Qt::Key_O);
    pOpenFileAction->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    pToolbar->addAction(pOpenFileAction);
    QAction *pOpenFolderAction = fileMenu->addAction("Open &folder...", this, &MainWindow::onOpenFolderTriggered);
    pOpenFolderAction->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));
    pToolbar->addAction(pOpenFolderAction);
    fileMenu->addSeparator();

    // opening start page
    fileMenu->addAction("Start &Page...", this, &MainWindow::onOpenStartPage);
    fileMenu->addSeparator();

    // saving files
    QAction *pSaveAction = fileMenu->addAction("&Save...", this, &MainWindow::onSaveFileTriggered, Qt::CTRL + Qt::Key_S);
    pSaveAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    pToolbar->addAction(pSaveAction);
    fileMenu->addAction("Save &As...", this, &MainWindow::onSaveFileAsTriggered);
    fileMenu->addAction("Save A&ll...", this, &MainWindow::onSaveAllFilesTriggered, Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    fileMenu->addSeparator();

    // closing docs & exiting the program
    fileMenu->addAction("&Close document", this, &MainWindow::onCloseFileTriggered, Qt::CTRL + Qt::SHIFT + Qt::Key_W);
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
    QAction *pFindAction = editMenu->addAction("&Find/Replace...", this, &MainWindow::onFindTriggered, Qt::CTRL + Qt::Key_F);
    pFindAction->setDisabled(true);

    // view menu
    QMenu *viewMenu = new QMenu("&View");
    QAction *pFullScreenActoin = viewMenu->addAction("&Full screen", this, &MainWindow::onFullScreenTriggered, Qt::CTRL + Qt::SHIFT + Qt::Key_F11);
    pFullScreenActoin->setDisabled(true);

    QMenu *scaleSubMenu = new QMenu("&Scale");
    QAction *pScaleSubMenu = viewMenu->addMenu(scaleSubMenu);
    pScaleSubMenu->setDisabled(true);

    viewMenu->addSeparator();
    viewMenu->addAction("Show &Project Viewer", this, &MainWindow::onShowProjectViewerTriggered);
    viewMenu->addAction("Show &Chat Window", this, &MainWindow::onShowChatWindowDockTriggered);
    viewMenu->addAction("Show &Bottom Panel", this, &MainWindow::onShowBottomPanel);

    // tools menu
    QMenu *toolsMenu = new QMenu("&Tools");

    // opening refactoring toolbar
    QAction *pRefactorAction = toolsMenu->addAction("&Refactor...", this, &MainWindow::onRefactorTriggered);
    pRefactorAction->setDisabled(true);
    toolsMenu->addSeparator();

    // opening chat window
    QAction *pConnectAction = toolsMenu->addAction("&Connect...", this, &MainWindow::onConnectTriggered);
    pConnectAction->setIcon(style()->standardIcon(QStyle::SP_DialogYesButton));
    pToolbar->addAction(pConnectAction);
    toolsMenu->addSeparator();

    // buidling solution
    QMenu *buildSubMenu = new QMenu("&Build");
    QAction *pBuildActoin = toolsMenu->addMenu(buildSubMenu);
    pBuildActoin->setDisabled(true);

    // debugging program
    QMenu *debugSubMenu = new QMenu("&Debug");
    QAction *pDebugAction = toolsMenu->addMenu(debugSubMenu);
    pDebugAction->setDisabled(true);

    // git
    QMenu *gitSubMenu = new QMenu("&Git");
    QAction *pGitActoin = toolsMenu->addMenu(gitSubMenu);
    pGitActoin->setDisabled(true);
    toolsMenu->addSeparator();

    // opening settings window
    QAction *pSettingsAction = toolsMenu->addAction("&Settings...", this, &MainWindow::onSettingsTriggered);
    pSettingsAction->setDisabled(true);

    // help menu
    QMenu *helpMenu = new QMenu("&Help");

    // info about program
    helpMenu->addAction("&About...", this, &MainWindow::onAboutTriggered);
    helpMenu->addSeparator();

    // opening reference window
    helpMenu->addAction("&Reference Assistant...", this, &MainWindow::onReferenceTriggered, Qt::CTRL + Qt::Key_F1);

    // user guide
    QAction *pUserGuideActoin = helpMenu->addAction("User &Guide...", this, &MainWindow::onUserGuideTriggered, Qt::Key_F1);
    pUserGuideActoin->setDisabled(true);
    helpMenu->addSeparator();

    // checking updates
    QAction *pUpdatesAction = helpMenu->addAction("Check for &Updates...", this, &MainWindow::onCheckUpdatesTriggered);
    pUpdatesAction->setDisabled(true);

    // adding all menus to MainMenuBar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(helpMenu);

    addToolBar(Qt::TopToolBarArea, pToolbar);
}

void MainWindow::saveDocument(CodeEditor *pDoc, const QString &fileName)
{    
    try
    {
        // writing to file
        FileManager().writeToFile(fileName, pDoc->toPlainText());
        statusBar()->showMessage(tr("Changes to document have been saved"), 3000);
    } catch (const QException&)
    {
        QMessageBox::warning(this, "Error", "Unable to open file for saving");
    }
    pDoc->document()->setModified(false);
}

void MainWindow::openDoc(QString fileName)
{
    QString readResult;

    try
    {
        readResult = FileManager().readFromFile(fileName);
    }
    catch (const IncorrectUserInput&)
    {
        return;
    }
    catch (const QException&)
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

bool MainWindow::checkIfOpened(const QString &fileName) const
{
    // getting all docs
    auto docsList = mpDocsArea->subWindowList();

    for (const auto &doc : docsList)
    {
        auto curDoc = qobject_cast<CodeEditor*>(doc->widget());
        if (curDoc && curDoc->getFileName() == fileName)
        {
            return true;
        }
    }

    return false;
}

bool MainWindow::checkIfModified(QList<QMdiSubWindow*> &docsList)
{
    for (int i = 0; i < docsList.size(); ++i)
    {
        auto curDoc = qobject_cast<CodeEditor*>(docsList[i]->widget());

        if (curDoc && curDoc->document()->isModified())
        {
            return true;
        }
    }
    return false;
}

void MainWindow::saveAllModifiedDocuments(QList<QMdiSubWindow*> &docsList)
{
    for (int i = 0; i < docsList.size(); ++i)
    {
        auto curDoc = qobject_cast<CodeEditor*>(docsList[i]->widget());
        saveDocument(curDoc, curDoc->getFileName());
    }
}

void MainWindow::createProjectViewer()
{
    mpProjectViewerDock = new ProjectViewerDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, mpProjectViewerDock);
    mpProjectViewerDock->setObjectName("mpProjectViewerDock");
}

void MainWindow::createChatWindow()
{
    // create instance of Chat Window
    mpChatWindowDock = new ChatWindowDock(this);
    connect(mpChatWindowDock, &ChatWindowDock::userToConnectSelected,
            this, &MainWindow::onUserToConnectSelected);
    connect(mpChatWindowDock, &ChatWindowDock::sendMessage,
            this, &MainWindow::onSendMessage);

    mpChatWindowDock->setObjectName("mpChatWindowDock");
    addDockWidget(Qt::RightDockWidgetArea, mpChatWindowDock, Qt::Vertical);
}

void MainWindow::createButtomPanel()
{
    // create instance of Bottom Panel
    mpBottomPanelDock = new BottomPanelDock(this);
    mpBottomPanelDock->setObjectName("mpBottomPanelDock");
}

CodeEditor *MainWindow::getCurrentDoc()
{
    // get current subWindow
    // if there are no subWindows nullptr is returned
    auto subWindow = mpDocsArea->currentSubWindow();

    if (!subWindow)
    {
        return nullptr;
    }
    auto curDoc = qobject_cast<CodeEditor*>(subWindow->widget());

    return curDoc ? curDoc : nullptr;
}

void MainWindow::onNewFileTriggered()
{    
    QStringList fileExtensions = getFileExtensions();
    NewFileDialog newFileDialog(fileExtensions, this);
    QString newFileName;
    try
    {
        // new file dialog is called
        // name of newly created file is received
        newFileName = newFileDialog.start();
    }
    catch (const QException&)
    {
        return;
    }

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
            (this,
             "Open File",
             QDir::currentPath(),
             "C++/C files (*.h *.hpp *.cpp *.c) ;; Text Files (*.txt) ;; JSON Files (*.json)");

    // if document already opened then return
    if (checkIfOpened(fileName))
    {
        QMessageBox::warning(this, "Document already opened", "Selected document already opened.");
        return;
    }

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
    if (!mpDocsArea->currentSubWindow())
    {
        QMessageBox::information(this, "Save", "There are no opened documents to save.");
        return;
    }

    auto curDoc = getCurrentDoc();

    // if doc wasn't modified yet
    if (!curDoc || !curDoc->document()->isModified())
    {
        return;
    }
    // saving doc
    saveDocument(curDoc, curDoc->getFileName());
}

void MainWindow::onSaveFileAsTriggered()
{
    // if there are no opened docs
    if (!mpDocsArea->currentSubWindow())
    {
        QMessageBox::information(this, "Save", "There are no opened documents to save.");
        return;
    }

    auto curDoc = getCurrentDoc();

    // if there are no opened docs
    if (!curDoc)
    {
        return;
    }
    QString extension;

    QString fileName = QFileDialog::getSaveFileName
            (this,
             "Save As",
             QDir::currentPath() + "/Unnamed",
             "*.h ;; *.hpp ;; *.cpp ;; *.c ;; *.txt ;; *.json",
             &extension);

    // if user closed dialog
    if (fileName.isEmpty())
    {
        return;
    }

    int position = fileName.indexOf(QChar{'.'});
    fileName += extension.mid(position + 1);

    // saving doc
    saveDocument(curDoc, fileName);
}

void MainWindow::onSaveAllFilesTriggered()
{
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

        if(curDoc && curDoc->document()->isModified())
        {
            saveDocument(curDoc, curDoc->getFileName());
        }
    }
}

void MainWindow::onCloseFileTriggered()
{    
    auto curDoc = getCurrentDoc();

    // if there are no opened documents
    if (!curDoc)
    {
        return;
    }
    // if doc wasn't modified then just close doc
    if (!curDoc->document()->isModified())
    {
        mpDocsArea->closeActiveSubWindow();
        return;
    }

    // ask user whether changes should be saved
    QMessageBox::StandardButton reply
            = QMessageBox::question
            (this,
             "Save changes",
             "Do you want to save changes to current document?",
             QMessageBox::StandardButton::Yes |
             QMessageBox::StandardButton::No |
             QMessageBox::StandardButton::Cancel);

    // checking user's answer
    if (reply == QMessageBox::Yes)
    {
        saveDocument(curDoc, curDoc->getFileName());
    }
    if (reply == QMessageBox::Cancel)
    {
        return;
    }

    // closing doc
    mpDocsArea->closeActiveSubWindow();
}

void MainWindow::onExitTriggered()
{
    QApplication::closeAllWindows();
}

void MainWindow::onUndoTriggered()
{
    auto curDoc = getCurrentDoc();

    if (curDoc)
    {
        curDoc->undo();
    }
}

void MainWindow::onRedoTriggered()
{
    auto curDoc = getCurrentDoc();

    if (curDoc)
    {
        curDoc->redo();
    }
}

void MainWindow::onCutTriggered()
{
    auto curDoc = getCurrentDoc();

    if (curDoc)
    {
        curDoc->cut();
    }
}

void MainWindow::onCopyTriggered()
{
    auto curDoc = getCurrentDoc();

    if (curDoc)
    {
        curDoc->copy();
    }
}

void MainWindow::onPasteTriggered()
{
    auto curDoc = getCurrentDoc();

    if (curDoc)
    {
        curDoc->paste();
    }
}

void MainWindow::onSelectAllTriggered()
{    
    auto curDoc = getCurrentDoc();

    if (curDoc)
    {
        curDoc->selectAll();
    }
}

void MainWindow::onFindTriggered()
{
    //
}

void MainWindow::onFullScreenTriggered()
{
    //
}

void MainWindow::onShowProjectViewerTriggered()
{
    mpProjectViewerDock->show();
}

void MainWindow::onShowChatWindowDockTriggered()
{
    mpChatWindowDock->show();
}

void MainWindow::onShowBottomPanel()
{
    mpBottomPanelDock->show();
}

void MainWindow::onRefactorTriggered()
{
    //
}

void MainWindow::onConnectTriggered()
{
    LoginDialog loginDialog(this);
    QString userInput = loginDialog.start();
    if (userInput.isEmpty())
    {
        return;
    }
    mCurrentUserName = userInput;
    mpChatWindowDock->setUserName(userInput);
}

void MainWindow::onSettingsTriggered()
{
    //
}

void MainWindow::onAboutTriggered()
{
    QString info = "This application has been "
                   "developed by students of group LV-425.C++ of SoftServe IT Academy. "
                   "\n\nIt is designed to provide tools for "
                   "pair programming as well as facilities "
                   "of high-end development environment.";
    QMessageBox::about(this, "About PairStorm", info);
}

void MainWindow::onReferenceTriggered()
{
    //
}

void MainWindow::onUserGuideTriggered()
{
    //
}

void MainWindow::onCheckUpdatesTriggered()
{
    //
}

void MainWindow::onOpenFileFromProjectViewer(QString fileName)
{
    // if document already opened then return
    if (checkIfOpened(fileName))
    {
        QMessageBox::warning(this, "Document already opened", "Selected document already opened.");
        return;
    }

    openDoc(fileName);
}

void MainWindow::onCloseWindow(CodeEditor *curDoc)
{
    saveDocument(curDoc, curDoc->getFileName());
}

void MainWindow::onUserToConnectSelected(QString userName)
{
    //
}

void MainWindow::onNewMessage(const QString &userName, const QString &message)
{
    mpChatWindowDock->displayMessage(userName, message);
}

void MainWindow::onSendMessage(const QString &userName, const QString &message)
{
    //
}

void MainWindow::onConnectionFailed()
{
    QMessageBox::warning(this, "Connection error", "Connection failed");
}

CodeEditor* MainWindow::createNewDoc()
{
    CodeEditor *newDoc = new CodeEditor;

    mpDocsArea->addSubWindow(newDoc);
    connect(newDoc, &CodeEditor::closeDocEventOccured, this, &MainWindow::onCloseWindow);
    newDoc->setAttribute(Qt::WA_DeleteOnClose);

    return newDoc;
}

void MainWindow::closeEvent(QCloseEvent *event)
{    
    // getting all docs
    auto docsList = mpDocsArea->subWindowList();

    // if there are no docs
    if(docsList.empty())
    {
        event->accept();
        return;
    }

    // if doc is modified then we should ask user if changes have to be saved
    if(!checkIfModified(docsList))
    {
        event->accept();
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question
            (this,
             "Saving Changes",
             "Do you want to save changes to opened documents?",
             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::No)
    {
        event->accept();
        return;
    }
    if (reply == QMessageBox::Yes)
    {
        // if appreved then save changes
        saveAllModifiedDocuments(docsList);
        event->accept();
        return;
    }

    event->ignore();
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

void MainWindow::setAppStyle()
{
    QString styleName = "Fusion";
    QStringList availableStyles = QStyleFactory::keys();
    if (availableStyles.contains(styleName))
    {
        QApplication::setStyle(QStyleFactory::create(styleName));
    }
}
