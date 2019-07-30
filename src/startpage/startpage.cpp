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

    int maxButtonsWidth = 200;

    // creating & configuring new file button
    pNewBtn = new QPushButton(tr("   New File"));
    setupButton(pNewBtn, style()->standardIcon(QStyle::SP_FileIcon), maxButtonsWidth);
    connect(pNewBtn, &QPushButton::pressed, this, &StartPage::newBtnPressed);

    // creating & configuring open file button
    pOpenBtn = new QPushButton(tr("   Open File"));
    setupButton(pOpenBtn, style()->standardIcon(QStyle::SP_DialogOpenButton), maxButtonsWidth);
    connect(pOpenBtn, &QPushButton::pressed, this, &StartPage::openBtnPressed);

    // creating & configuring open folder button
    pOpenDirBtn = new QPushButton(tr("   Open Folder"));
    setupButton(pOpenDirBtn, style()->standardIcon(QStyle::SP_DirHomeIcon), maxButtonsWidth);
    connect(pOpenDirBtn, &QPushButton::pressed, this, &StartPage::openDirPressed);

    // creating & configuring reference call button
    pReferenceBtn = new QPushButton(tr("   Reference Assistant"));
    setupButton(pReferenceBtn, style()->standardIcon(QStyle::SP_DialogHelpButton), maxButtonsWidth);
    connect(pReferenceBtn, &QPushButton::pressed, this, &StartPage::referenceBtnPressed);

    // creating & configuring settings call button
    pSettingsBtn = new QPushButton(tr("   Settings"));
    setupButton(pSettingsBtn, style()->standardIcon(QStyle::SP_BrowserReload), maxButtonsWidth);
    connect(pSettingsBtn, &QPushButton::pressed, this, &StartPage::settingsBtnPressed);

    // bindong buttons
    QVBoxLayout *pBtnLayout = new QVBoxLayout;
    pBtnLayout->addWidget(pNewBtn);
    pBtnLayout->addWidget(pOpenBtn);
    pBtnLayout->addWidget(pOpenDirBtn);
    pBtnLayout->addWidget(pReferenceBtn);
    pBtnLayout->addWidget(pSettingsBtn);

    // creating & laying out labels
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

    // window lay out
    QHBoxLayout *pWdwLayout = new QHBoxLayout;
    pWdwLayout->addLayout(pBtnLayout);
    pWdwLayout->addLayout(pLblLayout);

    setLayout(pWdwLayout);

    // resizing & centring dialog
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

void StartPage::setupButton(QPushButton *pButton,
                            QIcon icon,
                            int maxWidth)
{
    pButton->setIcon(icon);
    pButton->setMaximumWidth(maxWidth);
    pButton->setSizePolicy(QSizePolicy::Expanding,
                           QSizePolicy::Expanding);

}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::newBtnPressed()
{
    emit onNewBtnPressed();
    accept();
}

void StartPage::openBtnPressed()
{
    emit onOpenBtnPressed();
    accept();
}

void StartPage::openDirPressed()
{
    emit onOpenDirPressed();
    accept();
}

void StartPage::referenceBtnPressed()
{
    emit onReferenceBtnPressed();
    accept();
}

void StartPage::settingsBtnPressed()
{
    emit onSettingsBtnPressed();
    accept();
}
