#include "startpage.h"
#include "ui_startpage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStyle>

StartPage::StartPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartPage)
{    
    ui->setupUi(this);
    setWindowTitle("Start Page");    

    pNewBtn = new QPushButton(tr("   New File"));
    pNewBtn->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    pNewBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pNewBtn->setMaximumWidth(200);
    connect(pNewBtn, &QPushButton::pressed, this, &StartPage::newBtnPressed);

    pOpenBtn = new QPushButton(tr("   Open File"));
    pOpenBtn->setMaximumWidth(200);
    pOpenBtn->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    pOpenBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pOpenBtn, &QPushButton::pressed, this, &StartPage::openBtnPressed);

    pOpenDirBtn = new QPushButton(tr("   Open Folder"));
    pOpenDirBtn->setMaximumWidth(200);
    pOpenDirBtn->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));
    pOpenDirBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pOpenDirBtn, &QPushButton::pressed, this, &StartPage::openDirPressed);

    pReferenceBtn = new QPushButton(tr("   Reference Assistant"));
    pReferenceBtn->setMaximumWidth(200);
    pReferenceBtn->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));
    pReferenceBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pReferenceBtn, &QPushButton::pressed, this, &StartPage::referenceBtnPressed);

    pSettingsBtn = new QPushButton(tr("   Settings"));
    pSettingsBtn->setMaximumWidth(200);
    pSettingsBtn->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    pSettingsBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pSettingsBtn, &QPushButton::pressed, this, &StartPage::settingsBtnPressed);

    QVBoxLayout *pBtnLayout = new QVBoxLayout;
    pBtnLayout->addWidget(pNewBtn);
    pBtnLayout->addWidget(pOpenBtn);
    pBtnLayout->addWidget(pOpenDirBtn);
    pBtnLayout->addWidget(pReferenceBtn);
    pBtnLayout->addWidget(pSettingsBtn);

    QLabel *pNewLbl = new QLabel(tr("Create new file"));
    pNewLbl->setAlignment(Qt::AlignCenter);
    QLabel *pOpenLbl = new QLabel(tr("Open existing file"));
    pOpenLbl->setAlignment(Qt::AlignCenter);
    QLabel *pOpenDirLbl = new QLabel(tr("Open existing project directory"));
    pOpenDirLbl->setAlignment(Qt::AlignCenter);
    QLabel *pReferenceLbl = new QLabel(tr("Open Reference Assistant"));
    pReferenceLbl->setAlignment(Qt::AlignCenter);
    QLabel *pSettingsLbl = new QLabel(tr("Configure IDE"));
    pSettingsLbl->setAlignment(Qt::AlignCenter);

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
}

void StartPage::showStartPage()
{
    exec();
}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::newBtnPressed()
{
    emit onNewBtnPressed();
}

void StartPage::openBtnPressed()
{
    emit onOpenBtnPressed();
}

void StartPage::openDirPressed()
{
    emit onOpenDirPressed();
}

void StartPage::referenceBtnPressed()
{
    emit onReferenceBtnPressed();
}

void StartPage::settingsBtnPressed()
{
    emit onSettingsBtnPressed();
}
