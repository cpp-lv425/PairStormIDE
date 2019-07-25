#include "startpage.h"
#include "ui_startpage.h"

#include <QStyleFactory>
#include <QListWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QStyle>

StartPage::StartPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartPage)
{    
    ui->setupUi(this);
    setWindowTitle("Start Page");
    mpStartPageList = new QListWidget;
    mpStartPageList->setFont(QFont("Verdana", 16));

    mpStartPageList->addItem(tr("Create new file"));
    mpStartPageList->addItem(tr("Open existing file"));
    mpStartPageList->addItem(tr("Open project directory"));
    mpStartPageList->addItem(tr("Open Reference Assistant"));
    mpStartPageList->addItem(tr("Configure IDE"));

    mpStartPageList->item(0)->setIcon(style()->
                                      standardIcon(QStyle::SP_FileIcon));
    mpStartPageList->item(1)->setIcon(style()->
                                      standardIcon(QStyle::SP_DialogOpenButton));
    mpStartPageList->item(2)->setIcon(style()->
                                      standardIcon(QStyle::SP_DirHomeIcon));
    mpStartPageList->item(3)->setIcon(style()->
                                      standardIcon(QStyle::SP_DialogHelpButton));
    mpStartPageList->item(4)->setIcon(style()->
                                      standardIcon(QStyle::SP_BrowserReload));

    setGeometry(geometry().x(), geometry().y(), 400, 200);

    QPoint cntr = parent->geometry().center();
    int x = cntr.x() - width() / 2;
    int y = cntr.y() - height() / 2;
    move(x, y);

    QVBoxLayout *pWdwLayout = new QVBoxLayout;
    pWdwLayout->addWidget(mpStartPageList);

    setLayout(pWdwLayout);
    exec();
}

StartPage::~StartPage()
{
    delete ui;
}


