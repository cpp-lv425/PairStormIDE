#include "browserdialog.h"
#include "ui_browserdialog.h"

#include <QDir>
#include <QScreen>
#include <QSizePolicy>
#include <QMdiSubWindow>
#include <QStandardPaths>
#include <QWebEngineView>
#include <QWebEnginePage>

#include "mdiarea.h"
#include "documentationsearch.h"
#include "connectionmanager.h"
#include "documentationengine.h"
#include "documentationviewer.h"
#include "htmlcontentgenerator.h"

BrowserDialog::BrowserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrowserDialog)
{
    ui->setupUi(this);

    // receive current screen geometry
    QScreen *screen = qApp->screens().at(0);
    QRect screenGeometry = screen->geometry();

    // resizing & centring dialog
    resize(screenGeometry.width() / 2,
           screenGeometry.height() / 2);

    mConnectionManager = new ConnectionManager(this);
    ui->mMDIArea->setViewMode(QMdiArea::ViewMode::TabbedView);
}

BrowserDialog::~BrowserDialog()
{
    delete ui;
}

void BrowserDialog::createNewTab(const QString &keyword)
{
    DocumentationViewer *newWindow = new DocumentationViewer(this);
    newWindow->loadReferenceDocumentation(keyword);
    auto temp = ui->mMDIArea->addSubWindow(newWindow);
    temp->setWindowState(Qt::WindowState::WindowMaximized);
    newWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void BrowserDialog::createEmptyTab()
{
    DocumentationViewer *newWindow = new DocumentationViewer(this);
    newWindow->loadReferenceDocumentation();
    auto temp = ui->mMDIArea->addSubWindow(newWindow);
    temp->setWindowState(Qt::WindowState::WindowMaximized);
    newWindow->setAttribute(Qt::WA_DeleteOnClose);
}

