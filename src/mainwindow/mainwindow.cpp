#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPlainTextEdit> // temporarily included
#include <QStyleFactory>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout> // temporarily included
#include <QDebug> // temporarily included
#include <QFile>

#include "projectviewerdock.h"
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

    setMainMenu();

    // create instance of Project Viewer
    mpProjectViewerDock = new ProjectViewerDock(this);

    // create instance of MDIArea
    mpDocsArea = new MDIArea(this);
    setCentralWidget(mpDocsArea);
}

void MainWindow::setMainMenu()
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enter new filename"), QDir::homePath());
    QWidget *newDoc = createNewDoc();
    newDoc->setWindowTitle(fileName);
    createFile(fileName);
    newDoc->show();
}

void MainWindow::onOpenFileTriggered()
{
    QWidget *newDoc = createNewDoc();
    QString readResult = loadFile();

    // current interface is not implemented by Code Editor Widget yet

//    if(!readResult.isEmpty())
//        newDoc->setText(readResult);

    // current block will be removed when Code Editor Widget will be implemented
    if(!readResult.isEmpty())
    {
        QPlainTextEdit *pTextEdit = new QPlainTextEdit;
        pTextEdit->setPlainText(readResult);
        QVBoxLayout *pLayout = new QVBoxLayout;
        pLayout->addWidget(pTextEdit);
        newDoc->setLayout(pLayout);
    }

    newDoc->show();
}

void MainWindow::onOpenFolderTriggered()
{
    qDebug() << "open folder";
}

void MainWindow::onOpenStartPage()
{
    qDebug() << "open start page";
}

void MainWindow::onSaveFileTriggered()
{
    qDebug() << "save file";
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

QWidget* MainWindow::createNewDoc()
{
    QWidget *newDoc = new QWidget;
    mpDocsArea->addSubWindow(newDoc);
    newDoc->setAttribute(Qt::WA_DeleteOnClose);

    return newDoc;
}

QString MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName();

    if (fileName.isEmpty())
    {
        return QString();
    }

    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString readResult = stream.readAll();
        file.close();
        return readResult;
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Unable to open specified file."));
        return QString();
    }
}

void MainWindow::createFile(const QString& fname)
{
    QFile file(fname);

    if (file.open(QIODevice::WriteOnly))
    {
        file.close();
        return;
    }

    QMessageBox::warning(this, tr("Error"), tr("Unable to create file."));
}

MainWindow::~MainWindow()
{
    delete ui;
}
