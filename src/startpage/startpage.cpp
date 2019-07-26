#include "startpage.h"
#include "ui_startpage.h"

#include <QStyleFactory>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QStyle>
#include <QDebug>

StartPage::StartPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartPage)
{    
    ui->setupUi(this);
    setWindowTitle("Start Page");    

//    auto stylesList = QStyleFactory::keys();
//    setStyle(QStyleFactory::create(stylesList[0]));
//    mpStartPageList = new QListWidget;
//    mpStartPageList->setFont(QFont("Verdana", 16));

//    mpStartPageList->addItem(tr("Create new file"));
//    mpStartPageList->addItem(tr("Open existing file"));
//    mpStartPageList->addItem(tr("Open project directory"));
//    mpStartPageList->addItem(tr("Open Reference Assistant"));
//    mpStartPageList->addItem(tr("Configure IDE"));

//    mpStartPageList->item(0)->setIcon(style()->
//                                      standardIcon(QStyle::SP_FileIcon));
//    mpStartPageList->item(1)->setIcon(style()->
//                                      standardIcon(QStyle::SP_DialogOpenButton));
//    mpStartPageList->item(2)->setIcon(style()->
//                                      standardIcon(QStyle::SP_DirHomeIcon));
//    mpStartPageList->item(3)->setIcon(style()->
//                                      standardIcon(QStyle::SP_DialogHelpButton));
//    mpStartPageList->item(4)->setIcon(style()->
//                                      standardIcon(QStyle::SP_BrowserReload));

//    QGroupBox *pGroupBox = new QGroupBox("");
//    QVBoxLayout *pGroupLayout = new QVBoxLayout;
//    pGroupLayout->addWidget(mpStartPageList);
//    pGroupBox->setLayout(pGroupLayout);

//    QHBoxLayout *pMiddleLayout = new QHBoxLayout;
//    pMiddleLayout->addSpacing(50);
//    pMiddleLayout->addWidget(pGroupBox);
//    pMiddleLayout->addSpacing(50);

//    QVBoxLayout *pWdwLayout = new QVBoxLayout;
//    pWdwLayout->addStretch(1);
//    pWdwLayout->addLayout(pMiddleLayout);
//    pWdwLayout->addStretch(1);
//    setLayout(pWdwLayout);

    ////////////////////////////////////////

    QPushButton *pNewBtn = new QPushButton(tr("New File"));
    QPushButton *pOpenBtn = new QPushButton(tr("Open File"));
    QPushButton *pOpenDirBtn = new QPushButton(tr("Open Folder"));
    QPushButton *pReferenceBtn = new QPushButton(tr("Reference Assistant"));
    QPushButton *pSettingsBtn = new QPushButton(tr("Settings"));

    QVBoxLayout *pBtnLayout = new QVBoxLayout;
    pBtnLayout->addWidget(pNewBtn);
    pBtnLayout->addWidget(pOpenBtn);
    pBtnLayout->addWidget(pOpenDirBtn);
    pBtnLayout->addWidget(pReferenceBtn);
    pBtnLayout->addWidget(pSettingsBtn);

    QLabel *pNewLbl = new QLabel(tr("Create new file"));
    QLabel *pOpenLbl = new QLabel(tr("Open existing file"));
    QLabel *pOpenDirLbl = new QLabel(tr("Open existing project directory"));
    QLabel *pReferenceLbl = new QLabel(tr("Open Reference Assistant"));
    QLabel *pSettingsLbl = new QLabel(tr("Configure IDE"));
    QVBoxLayout *pLblLayout = new QVBoxLayout;
    pLblLayout->addWidget(pNewLbl);
    pLblLayout->addWidget(pOpenLbl);
    pLblLayout->addWidget(pOpenDirLbl);
    pLblLayout->addWidget(pReferenceLbl);
    pLblLayout->addWidget(pSettingsLbl);

    QHBoxLayout *pWdwLayout = new QHBoxLayout;
    pWdwLayout->addLayout(pBtnLayout);
    pWdwLayout->addLayout(pLblLayout);

    setLayout(pWdwLayout);

    setGeometry(geometry().x(), geometry().y(), 500, 300);

    QPoint cntr = parent->geometry().center();
    int x = cntr.x() - width() / 2;
    int y = cntr.y() - height() / 2;
    move(x, y);

    exec();
}

StartPage::~StartPage()
{
    delete ui;
}


