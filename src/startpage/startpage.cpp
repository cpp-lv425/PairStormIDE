#include "startpage.h"
#include "ui_startpage.h"

#include <QListWidget>
#include <QVBoxLayout>

StartPage::StartPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);
    mpStartPageList = new QListWidget;
    mpStartPageList->addItem(tr("Create new file"));
    mpStartPageList->addItem(tr("Open existing file"));
    mpStartPageList->addItem(tr("Open project directory"));
    mpStartPageList->addItem(tr("Open Reference Assistant"));
    mpStartPageList->addItem(tr("Configure IDE"));
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(mpStartPageList);
    mpStartPageList->show();
}

StartPage::~StartPage()
{
    delete ui;
}


