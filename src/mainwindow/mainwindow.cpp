#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidgetItem>
#include <QMdiSubWindow>
#include <QStyleFactory>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QSplitter>
#include <QStyle>
#include <QFile>

#include "localconnectorgenerator.h"
#include "settingsconfigurator.h"
#include "paletteconfigurator.h"
#include "projectviewerdock.h"
#include "newprojectwizard.h"
#include "documentmanager.h"
#include "bottompaneldock.h"
#include "savefilesdialog.h"
#include "classgenerator.h"
#include "chatwindowdock.h"
#include "newfilewizard.h"
#include "usermessages.h"
#include "logindialog.h"
#include "filemanager.h"
#include "menuoptions.h"
#include "codeeditor.h"
#include "storeconf.h"
#include "startpage.h"
#include "utils.h"
#include "sqliteaccess.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    // create instance of Document Manager
    mpDocumentManager(new DocumentManager),
    // initializing palette configurator with current palette
    mpPaletteConfigurator(new PaletteConfigurator(palette())),
    dbFileManager(new FileDb)
{
    // Generate default local network connector
    mplocalConnector =
            LocalConnectorGenerator::getDefaultConnector();
    // And output its state in case of changes
    connect(mplocalConnector,
            &LocalConnectorInterface::serviceStatusChanged,
            this,
            &MainWindow::onConnectionStatusChanged,
            Qt::UniqueConnection);

    ui->setupUi(this);
    {
        StoreConf conf;
        conf.restoreConFile();
    }

    // when first started main window is maximized
    setWindowState(Qt::WindowMaximized);

    // set icon
    setWindowIcon(QIcon(":/img/LOGO.png"));

    setWindowTitle("PairStorm");

    setupMainMenu();

    setCentralWidget(dynamic_cast<QWidget*>(mpDocumentManager->getSplitter()));

    // create instance of Project Viewer
    createProjectViewer();

    // create instance of Chat Window
    createChatWindow();

    // create instance of Bottom Panel
    createButtomPanel();

    setInitialAppStyle();
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
    connect(&startPage, &StartPage::onNewProjectBtnPressed, this, &MainWindow::onNewProjectTriggered);
    connect(&startPage, &StartPage::onOpenProjectBtnPressed, this, &MainWindow::onOpenProjectTriggered);
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

    // managing projects & files
    QMenu *newSubMenu = new QMenu("&New");
    QAction *pNewProjectAction = newSubMenu->addAction("New &project...", this, &MainWindow::onNewProjectTriggered);
    pNewProjectAction->setIcon(QIcon(":/img/NEWPROJECT.png"));
    pToolbar->addAction(pNewProjectAction);

    QAction *pNewFileAction = newSubMenu->addAction("&New file...", this, &MainWindow::onNewFileTriggered, Qt::CTRL + Qt::Key_N);
    pNewFileAction->setIcon(QIcon(":/img/NEWFILE.png"));
    pToolbar->addAction(pNewFileAction);

    QAction *pNewClassAction = newSubMenu->addAction("New &class...", this, &MainWindow::onNewClassTriggered);
    pNewClassAction->setIcon(QIcon(":/img/NEWCLASS.png"));
    pToolbar->addAction(pNewClassAction);

    fileMenu->addMenu(newSubMenu);

    QAction *pOpenFileAction = fileMenu->addAction("&Open file...", this, &MainWindow::onOpenFileTriggered, Qt::CTRL + Qt::Key_O);
    pOpenFileAction->setIcon(QIcon(":/img/OPENFILE.png"));
    pToolbar->addAction(pOpenFileAction);
    QAction *pOpenFolderAction = fileMenu->addAction("Open pro&ject...", this, &MainWindow::onOpenProjectTriggered);
    pOpenFolderAction->setIcon(QIcon(":/img/OPENDIR.png"));
    pToolbar->addAction(pOpenFolderAction);
    fileMenu->addSeparator();

    // opening start page
    fileMenu->addAction("Start &Page...", this, &MainWindow::onOpenStartPage);
    fileMenu->addSeparator();

    // saving files
    QAction *pSaveAction = fileMenu->addAction("&Save...", this, &MainWindow::onSaveFileTriggered, Qt::CTRL + Qt::Key_S);
    pSaveAction->setIcon(QIcon(":/img/SAVE.png"));
    pToolbar->addAction(pSaveAction);
    fileMenu->addAction("Save &As...", this, &MainWindow::onSaveFileAsTriggered);
    fileMenu->addAction("Save A&ll...", this, &MainWindow::onSaveAllFilesTriggered, Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    fileMenu->addSeparator();

    // closing docs, project & exiting the program
    fileMenu->addAction("Close pro&ject", this, &MainWindow::onCloseProjectTriggered, Qt::CTRL + Qt::ALT + Qt::Key_W);
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

    viewMenu->addAction("Split &Horizontally", this, &MainWindow::onSplitHorizontallyTriggered,
                        Qt::CTRL + Qt::Key_E);
    viewMenu->addAction("Split &Vectically", this, &MainWindow::onSplitVerticallyTriggered,
                        Qt::CTRL + Qt::SHIFT + Qt::Key_E);
    viewMenu->addAction("Co&mbine Document Areas", this, &MainWindow::onCombineAreas, Qt::ALT + Qt::SHIFT + Qt::Key_W);
    viewMenu->addAction("Close &Empty Document Area", this, &MainWindow::onCloseEmptyDocArea);

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
    pConnectAction->setIcon(QIcon(":/img/DISCONNECTED.png"));

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
    pSettingsAction->setIcon(QIcon(":/img/SETTINGS.png"));
    pToolbar->addAction(pSettingsAction);

    // add action connect to the toolbar after settings
    pToolbar->addAction(pConnectAction);

    // help menu
    QMenu *helpMenu = new QMenu("&Help");

    // info about program
    helpMenu->addAction("&About...", this, &MainWindow::onAboutTriggered);
    helpMenu->addSeparator();

    // opening reference window
    QAction *pReferenceAction = helpMenu->addAction("&Reference Assistant...", this, &MainWindow::onReferenceTriggered, Qt::CTRL + Qt::Key_F1);
    pReferenceAction->setIcon(QIcon(":/img/REFERENCEASSISTANT.png"));
    pToolbar->addAction(pReferenceAction);

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

void MainWindow::openDocument(const QString &fileName)
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
    catch (const FileOpeningFailure&)
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ErrorTitle],
                userMessages[UserMessages::FileOpeningErrorMsg]);
        return;
    }

    mpDocumentManager->openDocument(fileName, true);
}

void MainWindow::createProjectViewer()
{
    mpProjectViewerDock = new ProjectViewerDock(getFileExtensions(), this);
    addDockWidget(Qt::LeftDockWidgetArea, mpProjectViewerDock);
    connect(mpProjectViewerDock, &ProjectViewerDock::openFileFromProjectViewer,
            this, &MainWindow::onOpenFileFromProjectViewer);

    mpProjectViewerDock->setObjectName("mpProjectViewerDock");
}

void MainWindow::createChatWindow()
{
    // create instance of Chat Window
    mpChatWindowDock = new ChatWindowDock(this);

    // Add updating users list on discovering new users and connecting new users
    connect(mplocalConnector, &LocalConnectorInterface::onlineUsersUpdated,
            mpChatWindowDock, &ChatWindowDock::updateOnlineUsersOnChange,
            Qt::UniqueConnection);
    connect(mplocalConnector, &LocalConnectorInterface::connectedUsersUpdated,
            mpChatWindowDock, &ChatWindowDock::updateConnectedUsersOnChange,
            Qt::UniqueConnection);
    // Allow start sharing and stop sharing on user input
    connect(mpChatWindowDock, &ChatWindowDock::startSharingWithUser,
            mplocalConnector, &LocalConnectorInterface::startSharing,
            Qt::UniqueConnection);
    connect(mpChatWindowDock, &ChatWindowDock::stopSharingWithUser,
            mplocalConnector, &LocalConnectorInterface::stopSharing,
            Qt::UniqueConnection);
    // Allow sending and displaying messages
    connect(mpChatWindowDock, &ChatWindowDock::shareMessage,
            mplocalConnector, &LocalConnectorInterface::shareMessage,
            Qt::UniqueConnection);
    connect(mplocalConnector, &LocalConnectorInterface::messageReceived,
            mpChatWindowDock, &ChatWindowDock::pushMessageToChat,
            Qt::UniqueConnection);

    mpChatWindowDock->setObjectName("mpChatWindowDock");
    addDockWidget(Qt::RightDockWidgetArea, mpChatWindowDock, Qt::Vertical);
}

void MainWindow::createButtomPanel()
{
    // create instance of Bottom Panel
    mpBottomPanelDock = new BottomPanelDock(this);
    mpBottomPanelDock->setObjectName("mpBottomPanelDock");
    addDockWidget(Qt::BottomDockWidgetArea, mpBottomPanelDock);
}

void MainWindow::onNewFileTriggered()
{    
    // check if project is opened
    if (!mpDocumentManager->projectOpened())
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ProjectNotOpenedTitle],
                userMessages[UserMessages::ProjectNotOpenedMsg]);
        return;
    }

    QStringList fileExtensions = getFileExtensions();
    NewFileDialog newFileDialog
            (fileExtensions,
             mpDocumentManager->getCurrentProjectPath(),
             this);

    QString newFileName;
    try
    {
        // new file dialog is called
        // name of newly created file is received
        newFileName = newFileDialog.start();
        File newfile(newFileName);
        dbFileManager->addFileToDb(newfile);
    }
    catch (const QException&)
    {
        return;
    }

    try
    {
        mpDocumentManager->openDocument(newFileName);
    }
    catch (const QException&)
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ErrorTitle],
                userMessages[UserMessages::FileOpeningErrorMsg]);
    }
}

void MainWindow::onNewClassTriggered()
{
    // check if project is opened
    if (!mpDocumentManager->projectOpened())
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ProjectNotOpenedTitle],
                userMessages[UserMessages::ProjectNotOpenedMsg]);
        return;
    }

    // get path to opened project
    QString currentProjectDirectory = mpDocumentManager->getCurrentProjectPath();

    // implementation
    ClassGenerator *classGenerator = new ClassGenerator(currentProjectDirectory);
    classGenerator->show();
    connect(classGenerator, &ClassGenerator::filesWereCreated, this, &MainWindow::openCreatedClassFiles);


}

void MainWindow::onOpenFileTriggered()
{

    // check if project is opened
    if (!mpDocumentManager->projectOpened())
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ProjectNotOpenedTitle],
                userMessages[UserMessages::ProjectNotOpenedMsg]);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName
            (this,
             userMessages[UserMessages::OpenFileTitle],
            mpDocumentManager->getCurrentProjectPath(),
            "C++/C files (*.h *.hpp *.cpp *.c) ;; Text Files (*.txt) ;; JSON Files (*.json)");

    // in case user closed dialog without selecting file
    if (!fileName.size())
    {
        return;
    }

    // checks if selected file belongs to opened project
    if (!mpDocumentManager->fileBelongsToCurrentProject(fileName))
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::FileDoesNotBelongToProjectTitle],
                userMessages[UserMessages::FileDoesNotBelongToProjectMsg]);
        return;
    }

    openDocument(fileName);
}

void MainWindow::onOpenProjectTriggered()
{
    // check if other project is opened
    if (mpDocumentManager->projectOpened())
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ProjectOpenedTitle],
                userMessages[UserMessages::ProjectOpenedMsg]);
        return;
    }

     dirName = QFileDialog::getExistingDirectory
            (this,
             userMessages[UserMessages::OpenDirectoryTitle],
            QDir::homePath());

   /* Connection *db = ConnectionGetter::getDefaultConnection("C:/Users/Petro/Desktop/storage.db");
    CreateDB database;
    database.addTableFile();
    database.addTableUser();
    database.addTableComment();
    database.addTableMessage();*/


    // check if project exists
    if (!FileManager().projectExists(dirName))
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ProjectDoesNotExistTitle],
                userMessages[UserMessages::ProjectDoesNotExistMsg]);
        return;
    }
    restoreDatabaseFile();
    databaseConnect();

    mpProjectViewerDock->setDir(dirName);
    mpDocumentManager->openProject(dirName);
}

void MainWindow::onCloseProjectTriggered()
{
    // if no project is opened
    if (!mpDocumentManager->projectOpened())
    {
        return;
    }

    // check if opened documents were modified
    auto changedDocuments = mpDocumentManager->getChangedDocuments();

    if (changedDocuments.size())
    {
        QStringList changedDocsNames;

        for (const auto& doc: changedDocuments)
        {
            changedDocsNames << doc->getFileName();
        }

        SaveFilesDialog saveFilesDialog(changedDocsNames, this);

        // prompt user if changes to docs have to be saved
        switch (saveFilesDialog.start())
        {
        case QDialogButtonBox::StandardButton::YesToAll:
        {
            try
            {                
                // saving changes to opened documents
                mpDocumentManager->saveAllDocuments();
            } catch (const FileOpeningFailure&)
            {
                // if any of files could not be opened to save changes to
                // document then user is warned & action is interrupted
                QMessageBox::warning(this, userMessages[UserMessages::ErrorTitle],
                        userMessages[UserMessages::FileOpeningForSavingErrorMsg]);
                return;
            }
            break;
        }
        case QDialogButtonBox::StandardButton::NoToAll:
        {           
            break;
        }
        case QDialogButtonBox::StandardButton::Cancel:
        {            
            return;
        }
        default:
        {
            return;
        }
        }
    }
    // all documents are closed
    mpDocumentManager->closeAllDocumentsWithoutSaving();
    // project is closed
    mpDocumentManager->closeCurrentProject();
    mpProjectViewerDock->setDir(QDir::currentPath());

    // disconnect from db
    databaseDisconnect();
    hideDatabaseFile();

    QMessageBox::information
            (this,
             userMessages[UserMessages::ProjectClosedTitle],
            userMessages[UserMessages::ProjectClosedMsg]);
}

void MainWindow::onOpenStartPage()
{
    showStartPage();
}

void MainWindow::onSaveFileTriggered()
{
    try
    {
        // if document was modified, it is saved &
        // message is shown on the status bar
        if (mpDocumentManager->saveDocument())
        {
            statusBar()->showMessage(userMessages[UserMessages::DocumentSavedMsg], 3000);
        }
    }
    catch (const FileOpeningFailure&)
    {
        QMessageBox::information
                (this,
                 userMessages[UserMessages::ErrorTitle],
                userMessages[UserMessages::FileOpeningForSavingErrorMsg]);
    }
}

void MainWindow::onSaveFileAsTriggered()
{
    // receiving current document
    auto pCurrentDocument = mpDocumentManager->getCurrentDocument();

    // if there are no opened documents
    if (!pCurrentDocument)
    {        
        return;
    }

    QString extension;

    // prompt new filename from user
    QString fileName = QFileDialog::getSaveFileName
            (this,
             userMessages[UserMessages::SaveAsTitle],
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

    // saving document
    try
    {
        mpDocumentManager->saveDocumentAs(pCurrentDocument, fileName);
    }
    catch (const QException&)
    {
        QMessageBox::information
                (this,
                 userMessages[UserMessages::ErrorTitle],
                userMessages[UserMessages::CreatingFileFailureMsg]);
    }
}

void MainWindow::onSaveAllFilesTriggered()
{
    try
    {
        if (mpDocumentManager->saveAllDocuments())
        {
            statusBar()->showMessage(userMessages[UserMessages::DocumentSavedMsg], 3000);
        }
    }
    catch (const FileOpeningFailure&)
    {
        QMessageBox::information
                (this,
                 userMessages[UserMessages::ErrorTitle],
                userMessages[UserMessages::FileOpeningForSavingErrorMsg]);
    }
}

void MainWindow::onCloseFileTriggered()
{    
    mpDocumentManager->closeCurrentDocument();
}

void MainWindow::onExitTriggered()
{
    QApplication::closeAllWindows();
}

void MainWindow::onUndoTriggered()
{
    std::function<void(CodeEditor*)> functor = &CodeEditor::undo;
    mpDocumentManager->applyChangesToCurrentDocument(functor);
}

void MainWindow::onRedoTriggered()
{
    std::function<void(CodeEditor*)> functor = &CodeEditor::redo;
    mpDocumentManager->applyChangesToCurrentDocument(functor);
}

void MainWindow::onCutTriggered()
{
    std::function<void(CodeEditor*)> functor = &CodeEditor::cut;
    mpDocumentManager->applyChangesToCurrentDocument(functor);
}

void MainWindow::onCopyTriggered()
{
    std::function<void(CodeEditor*)> functor = &CodeEditor::copy;
    mpDocumentManager->applyChangesToCurrentDocument(functor);
}

void MainWindow::onPasteTriggered()
{
    std::function<void(CodeEditor*)> functor = &CodeEditor::paste;
    mpDocumentManager->applyChangesToCurrentDocument(functor);
}

void MainWindow::onSelectAllTriggered()
{    
    std::function<void(CodeEditor*)> functor = &CodeEditor::selectAll;
    mpDocumentManager->applyChangesToCurrentDocument(functor);
}

void MainWindow::onFindTriggered()
{
    //
}

void MainWindow::onFullScreenTriggered()
{
    //
}

void MainWindow::onSplitHorizontallyTriggered()
{
    mpDocumentManager->onSplit(Qt::Horizontal);
}

void MainWindow::onSplitVerticallyTriggered()
{
    mpDocumentManager->onSplit(Qt::Vertical);
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

void MainWindow::onCombineAreas()
{
    mpDocumentManager->combineDocAreas();
}

void MainWindow::onCloseEmptyDocArea()
{
    mpDocumentManager->closeEmptyDocArea();
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
    mpChatWindowDock->setUserName(userInput);
    mplocalConnector->configureOnLogin(userInput);
    QSettings savedSettings(QApplication::organizationName(), QApplication::applicationName());
    QString styleName = {savedSettings.contains("style") ?
                         savedSettings.value("style").toString()
                         : "WHITE"};
    mpChatWindowDock->updateTheme(styleName);
}

void MainWindow::onSettingsTriggered()
{
    MenuOptions * menuOptions = new MenuOptions(this);
    connect(menuOptions, &MenuOptions::valuesChanged,
            this, &MainWindow::onSettingsChanged);
    Q_UNUSED(menuOptions)
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
    // Reference Assistant module has been cut off, so just idle
}

void MainWindow::onUserGuideTriggered()
{
    //
}

void MainWindow::onCheckUpdatesTriggered()
{
    //
}

void MainWindow::onReferenceFromEditor(const QString &keyword)
{
    // Reference Assistant module has been cut off, so just idle
}

void MainWindow::onOpenFileFromProjectViewer(QString fileName)
{    
    // check if project is opened
    if (!mpDocumentManager->getCurrentProjectPath().size())
    {
        QMessageBox::information
                (this,
                 userMessages[UserMessages::ProjectNotOpenedTitle],
                userMessages[UserMessages::ProjectNotOpenedMsg]);
        return;
    }

    if (!mpDocumentManager->fileBelongsToCurrentProject(fileName))
    {
        QMessageBox::information
                (this,
                 userMessages[UserMessages::FileDoesNotBelongToProjectTitle],
                userMessages[UserMessages::FileDoesNotBelongToProjectMsg]);
        return;
    }

    openDocument(fileName);
}

void MainWindow::onConnectionStatusChanged(bool status)
{
    if(!status)
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ConnectionFailureTitle],
                userMessages[UserMessages::ConnectionFailureMsg]);
    }
}

void MainWindow::onSettingsChanged(std::map<QString, QString> newValues)
{
    SettingsConfigurator settingsConfigurator;

    for (const auto& newValue: newValues)
    {
        auto functor = settingsConfigurator.getSettingsFunctor(newValue.first);
        functor(this, newValue.second);
    }
}

void MainWindow::openCreatedClassFiles(QString headerFile, QString sourceFile)
{
    openDocument(headerFile);
    openDocument(sourceFile);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    auto changedDocuments = mpDocumentManager->getChangedDocuments();

    if (!changedDocuments.size())
    {
        event->accept();
        return;
    }

    QStringList changedDocsNames;

    for (const auto& doc: changedDocuments)
    {
        changedDocsNames << doc->getFileName();
    }

    SaveFilesDialog saveFilesDialog(changedDocsNames, this);

    switch (saveFilesDialog.start())
    {
    case QDialogButtonBox::StandardButton::YesToAll:
    {
        try
        {
            mpDocumentManager->saveAllDocuments();
            event->accept();
        } catch (const FileOpeningFailure&)
        {
            // if any of files could not be opened to save changes to
            // document then user is warned & closeEvent is ignored
            QMessageBox::warning(this, userMessages[UserMessages::ErrorTitle],
                    userMessages[UserMessages::FileOpeningForSavingErrorMsg]);
            event->ignore();
        }
        return;
    }
    case QDialogButtonBox::StandardButton::NoToAll:
    {
        event->accept();
        return;
    }
    case QDialogButtonBox::StandardButton::Cancel:
    {
        event->ignore();
        return;
    }
    default:
    {
        event->ignore();
        break;
    }
    }
}

MainWindow::~MainWindow()
{
    saveMainWindowState();
    StoreConf conf;
    conf.saveConFile();

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
    if (settings.contains("mainWindowGeometry"))
        restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    if (settings.contains("mainWindowState"))
        restoreState(settings.value("mainWindowState").toByteArray());
}

void MainWindow::setInitialAppStyle()
{
    // fusion style is applied globally
    // if platform does not support fusion, default style is applied
    qApp->setStyle(QStyleFactory::create("Fusion"));

    // restores style palette set in previous app session
    // if the app is run for the first time then default style (WHITE) is set
    QSettings savedSettings(QApplication::organizationName(), QApplication::applicationName());
    QString styleName = {savedSettings.contains("style") ?
                         savedSettings.value("style").toString()
                         : "WHITE"};
    setAppStyle(styleName);
}

void MainWindow::setAppStyle(const QString &styleName)
{
    // dark style palette is created & set globally
    QPalette palette = mpPaletteConfigurator->getPalette(styleName);

    qApp->setPalette(palette);

    std::function<void(DocumentManager*, CodeEditor*, const QString&)> functor
            (&DocumentManager::setStyle);

    mpDocumentManager->configureDocuments(functor, styleName);

    mpChatWindowDock->updateTheme(styleName);
}

void MainWindow::setDocumentFontFamily(const QString &fontFamily)
{
    std::function<void(DocumentManager*, CodeEditor*, const QString&)> functor
            (&DocumentManager::setFontFamily);

    mpDocumentManager->configureDocuments(functor, fontFamily);
}

void MainWindow::setDocumentFontSize(const QString &fontSize)
{    
    std::function<void(DocumentManager*, CodeEditor*, const QString&)> functor
            (&DocumentManager::setFontSize);

    mpDocumentManager->configureDocuments(functor, fontSize);
}

void MainWindow::onNewProjectTriggered()
{
    // check if other project is opened
    if (mpDocumentManager->projectOpened())
    {
        QMessageBox::information
                (this,
                 userMessages[UserMessages::ProjectOpenedTitle],
                userMessages[UserMessages::ProjectOpenedMsg]);
        return;
    }

    NewProjectDialog newProjectDialog;
    QString dirName;
    try
    {
        // new project dialog is called
        // name & location of new project directory is received
        dirName = newProjectDialog.promptProjectNameFromUser();
    }
    catch (const QException&)
    {
        return;
    }

    // project file is created
    try
    {
        FileManager().createProjectFile(dirName);
    } catch (const FileOpeningFailure&)
    {
        QMessageBox::warning
                (this,
                 userMessages[UserMessages::ErrorTitle],
                userMessages[UserMessages::ProjectCreationFailureMsg]);
        return;
    }

    databaseConnect( );
    // document manager is sent a message about new project
    mpDocumentManager->openProject(dirName);

    // project is displayed on project viewer
    mpProjectViewerDock->setDir(dirName);
}

void MainWindow::databaseConnect()
{
    setDatabaseFileName();
    db = ConnectionGetter::getDefaultConnection(databaseFileName);
    CreateDB database;
    /*database.addTableFile();
    database.addTableUser();
    database.addTableComment();
    database.addTableMessage();*/
}

void MainWindow::databaseDisconnect()
{
    delete db;
}

void MainWindow::restoreDatabaseFile()
{
    setDatabaseFileName();
    QString projectFileName = dirName + pathSeparator + FileManager::getProjectFileName();
    readWriteFileContent(projectFileName, databaseFileName);
}

void MainWindow::hideDatabaseFile()
{
    QString projectFileName = dirName + pathSeparator + FileManager::getProjectFileName();
    readWriteFileContent(databaseFileName, projectFileName);
    QFile::remove(databaseFileName);
}

void MainWindow::setDatabaseFileName()
{
    databaseFileName = dirName;
    int position = dirName.lastIndexOf(QChar{pathSeparator});
    databaseFileName += pathSeparator;
    databaseFileName += dirName.mid(position + 1);
    databaseFileName += projectDatabaseExtension;
}

void MainWindow::readWriteFileContent(QString fileToReadName, QString fileToWriteName)
{
    QFile fileToRead(fileToReadName);
    QFile fileToWrite(fileToWriteName);

    fileToRead.open(QIODevice::ReadOnly);
    fileToWrite.open(QIODevice::WriteOnly);

    QByteArray data = fileToRead.readAll();
    fileToWrite.write(data);
    qDebug()<<data;
    fileToRead.close();
    fileToWrite.close();
}
