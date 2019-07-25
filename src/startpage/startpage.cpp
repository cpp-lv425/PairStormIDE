#include "startpage.h"
#include "ui_startpage.h"

#include <QListWidget>
#include <QVBoxLayout>

StartPage::StartPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);
    pStartPageList = new QListWidget;
    pStartPageList->addItem(tr("Create new file"));
    pStartPageList->addItem(tr("Open existing file"));
    pStartPageList->addItem(tr("Open project directory"));
    pStartPageList->addItem(tr("Open Reference Assistant"));
    pStartPageList->addItem(tr("Configure IDE"));
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(pStartPageList);
    pStartPageList->show();
}

StartPage::~StartPage()
{
    delete ui;
}


