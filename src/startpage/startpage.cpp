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
    QFont lblFont("Segoe UI", 12);

    //lblFont.setBold(true);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::blue);

    // creating & configuring new file button
    mpNewBtn = new QPushButton;
    setupButton(mpNewBtn,
                style()->standardIcon(QStyle::SP_FileIcon),
                maxButtonsWidth,
                "New File");
    connect(mpNewBtn, &QPushButton::pressed, this, &StartPage::newBtnPressed);

    // creating & configuring open file button
    mpOpenBtn = new QPushButton;
    setupButton(mpOpenBtn, style()->standardIcon
                (QStyle::SP_DialogOpenButton),
                maxButtonsWidth,
                "Open File");
    connect(mpOpenBtn, &QPushButton::pressed, this, &StartPage::openBtnPressed);

    // creating & configuring open folder button
    mpOpenDirBtn = new QPushButton;
    setupButton(mpOpenDirBtn,
                style()->standardIcon(QStyle::SP_DriveHDIcon),
                maxButtonsWidth,
                "Open Folder");
    connect(mpOpenDirBtn, &QPushButton::pressed, this, &StartPage::openDirPressed);

    // creating & configuring reference call button
    mpReferenceBtn = new QPushButton;
    setupButton(mpReferenceBtn,
                style()->standardIcon(QStyle::SP_DialogHelpButton),
                maxButtonsWidth,
                "Reference Assistant");
    connect(mpReferenceBtn, &QPushButton::pressed, this, &StartPage::referenceBtnPressed);

    // creating & configuring settings call button
    mpSettingsBtn = new QPushButton;
    setupButton(mpSettingsBtn,
                style()->standardIcon(QStyle::SP_BrowserReload),
                maxButtonsWidth,
                "Settings");
    mpSettingsBtn->setDisabled(true);
    connect(mpSettingsBtn, &QPushButton::pressed, this, &StartPage::settingsBtnPressed);

    // bindong buttons
    QVBoxLayout *pBtnLayout = new QVBoxLayout;
    pBtnLayout->addWidget(mpNewBtn);
    pBtnLayout->addWidget(mpOpenBtn);
    pBtnLayout->addWidget(mpOpenDirBtn);
    pBtnLayout->addWidget(mpReferenceBtn);
    pBtnLayout->addWidget(mpSettingsBtn);

    // creating & laying out labels
    QLabel *pNewLbl = new QLabel(tr("Create new file"));
    setupLabels(pNewLbl, lblFont, palette);

    QLabel *pOpenLbl = new QLabel(tr("Open existing file"));
    setupLabels(pOpenLbl, lblFont, palette);

    QLabel *pOpenDirLbl = new QLabel(tr("Open existing project directory"));
    setupLabels(pOpenDirLbl, lblFont, palette);

    QLabel *pReferenceLbl = new QLabel(tr("Open Reference Assistant"));
    setupLabels(pReferenceLbl, lblFont, palette);

    QLabel *pSettingsLbl = new QLabel(tr("Configure IDE"));
    setupLabels(pSettingsLbl, lblFont, palette);
    pSettingsLbl->setDisabled(true);

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
                            int maxWidth, const QString &text)
{
    pButton->setIcon(icon);
    pButton->setMaximumWidth(maxWidth);
    pButton->setSizePolicy(QSizePolicy::Expanding,
                           QSizePolicy::Expanding);
    pButton->setStyleSheet("text-align:left;");
    pButton->setLayout(new QGridLayout);
    QLabel *textLabel = new QLabel(text);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    pButton->layout()->addWidget(textLabel);
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

void StartPage::setupLabels(QLabel *pLabel,
                            QFont &lblFont,
                            QPalette &palette)
{
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setFont(lblFont);
    pLabel->setPalette(palette);
}
