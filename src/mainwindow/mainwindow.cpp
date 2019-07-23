#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStyleFactory>
#include <QDebug>

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
}

void MainWindow::setMainMenu()
{
    // file menu
    QMenu *fileMenu = new QMenu("&File");
    // working with files
    fileMenu->addAction("&New file", this, &MainWindow::newFileTriggered, Qt::CTRL + Qt::Key_N);
    fileMenu->addAction("&Open file...", this, &MainWindow::openFileTriggered, Qt::CTRL + Qt::Key_O);
    fileMenu->addAction("Open &folder...", this, &MainWindow::openFolderTriggered);
    fileMenu->addSeparator();

    // opening start page
    fileMenu->addAction("Start &Page...", this, &MainWindow::openStartPage);
    fileMenu->addSeparator();

    // saving files
    fileMenu->addAction("&Save...", this, &MainWindow::saveFileTriggered, Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("Save &As...", this, &MainWindow::saveFileAsTriggered, Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("Save A&ll...", this, &MainWindow::saveAllFilesTriggered, Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    fileMenu->addSeparator();

    // closing docs & exiting the program
    fileMenu->addAction("&Close file", this, &MainWindow::closeFileTriggered, Qt::CTRL + Qt::Key_W);
    fileMenu->addAction("&Exit", this, &MainWindow::close, Qt::ALT + Qt::Key_F4);

    // edit menu
    QMenu *editMenu = new QMenu("&Edit");

    // undo / redo
    editMenu->addAction("&Undo", this, &MainWindow::undoTriggered, Qt::CTRL + Qt::Key_Z);
    editMenu->addAction("&Redo", this, &MainWindow::redoTriggered, Qt::CTRL + Qt::Key_Y);
    editMenu->addSeparator();

    // working with clipboard
    editMenu->addAction("Cu&t", this, &MainWindow::cutTriggered, Qt::CTRL + Qt::Key_X);
    editMenu->addAction("&Copy", this, &MainWindow::copyTriggered, Qt::CTRL + Qt::Key_C);
    editMenu->addAction("&Paste", this, &MainWindow::pasteTriggered, Qt::CTRL + Qt::Key_V);
    editMenu->addAction("Select &All", this, &MainWindow::selectAllTriggered, Qt::CTRL + Qt::Key_A);
    editMenu->addSeparator();

    // find patterns in docs
    editMenu->addAction("&Find/Replace...", this, &MainWindow::findTriggered, Qt::CTRL + Qt::Key_F);

    // view menu
    QMenu *viewMenu = new QMenu("&View");
    viewMenu->addAction("&Full screen", this, &MainWindow::fullScreenTriggered, Qt::CTRL + Qt::SHIFT + Qt::Key_F11);
    QMenu *scaleSubMenu = new QMenu("&Scale");
    viewMenu->addMenu(scaleSubMenu);

    // tools menu
    QMenu *toolsMenu = new QMenu("&Tools");

    // opening refactoring toolbar
    toolsMenu->addAction("&Refactor...", this, &MainWindow::refactorTriggered);
    toolsMenu->addSeparator();

    // opening chat window
    toolsMenu->addAction("&Connect...", this, &MainWindow::connectTriggered);
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
    toolsMenu->addAction("&Settings...", this, &MainWindow::settingsTriggered);

    // help menu
    QMenu *helpMenu = new QMenu("&Help");

    // info about program
    helpMenu->addAction("&About...", this, &MainWindow::aboutTriggered);
    helpMenu->addSeparator();

    // opening reference window
    helpMenu->addAction("&Reference Assistant...", this, &MainWindow::referenceTriggered, Qt::CTRL + Qt::Key_F1);

    // user guide
    helpMenu->addAction("User &Guide...", this, &MainWindow::userGuideTriggered, Qt::Key_F1);
    helpMenu->addSeparator();

    // checking updates
    helpMenu->addAction("Check for &Updates...", this, &MainWindow::checkUpdatesTriggered);

    // adding all menus to MainMenuBar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::newFileTriggered()
{
    qDebug() << "newFileTriggered";
}

void MainWindow::openFileTriggered()
{
    qDebug() << "open file";
}

void MainWindow::openFolderTriggered()
{
    qDebug() << "open folder";
}

void MainWindow::openStartPage()
{
    qDebug() << "open start page";
}

void MainWindow::saveFileTriggered()
{
    qDebug() << "save file";
}

void MainWindow::saveFileAsTriggered()
{
    qDebug() << "save as";
}

void MainWindow::saveAllFilesTriggered()
{
    qDebug() << "save all";
}

void MainWindow::closeFileTriggered()
{
    qDebug() << "close file";
}

void MainWindow::undoTriggered()
{
    qDebug() << "undo";
}

void MainWindow::redoTriggered()
{
    qDebug() << "redo";
}

void MainWindow::cutTriggered()
{
    qDebug() << "cut";
}

void MainWindow::copyTriggered()
{
    qDebug() << "copy";
}

void MainWindow::pasteTriggered()
{
    qDebug() << "paste";
}

void MainWindow::selectAllTriggered()
{
    qDebug() << "select all";
}

void MainWindow::findTriggered()
{
    qDebug() << "find";
}

void MainWindow::fullScreenTriggered()
{
    qDebug() << "full screen";
}

void MainWindow::refactorTriggered()
{
    qDebug() << "refactor";
}

void MainWindow::connectTriggered()
{
    qDebug() << "connect";
}

void MainWindow::settingsTriggered()
{
    qDebug() << "settings";
}

void MainWindow::aboutTriggered()
{
    qDebug() << "about";
}

void MainWindow::referenceTriggered()
{
    qDebug() << "reference";
}

void MainWindow::userGuideTriggered()
{
    qDebug() << "user guide";
}

void MainWindow::checkUpdatesTriggered()
{
    qDebug() << "check updates";
}

MainWindow::~MainWindow()
{
    delete ui;
}
