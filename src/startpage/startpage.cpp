#include "startpage.h"
#include "ui_startpage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QScreen>
#include <QLabel>
#include <QStyle>

StartPage::StartPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartPage)
{    
    ui->setupUi(this);
    setWindowTitle("Start Page");

    const int maxButtonsWidth = 200;
    const int iconDimension = 32;

    QFont lblFont("Segoe UI", 10);
    lblFont.setBold(true);


    // creating & configuring new file button
    mpNewBtn = new QPushButton;
    setupButton(mpNewBtn,
                QIcon(":/img/NEWFILE.png"),
                iconDimension,
                maxButtonsWidth,
                "New File");
    connect(mpNewBtn, &QPushButton::pressed, this, &StartPage::newBtnPressed);

    // creating & configuring open file button
    mpOpenBtn = new QPushButton;
    setupButton(mpOpenBtn,
                QIcon(":/img/OPENFILE.png"),
                iconDimension,
                maxButtonsWidth,
                "Open File");
    connect(mpOpenBtn, &QPushButton::pressed, this, &StartPage::openBtnPressed);

    // creating & configuring open folder button
    mpOpenDirBtn = new QPushButton;
    setupButton(mpOpenDirBtn,
                QIcon(":/img/OPENDIR.png"),
                iconDimension,
                maxButtonsWidth,
                "Open Project");
    connect(mpOpenDirBtn, &QPushButton::pressed, this, &StartPage::openDirPressed);

    // creating & configuring settings call button
    mpSettingsBtn = new QPushButton;
    setupButton(mpSettingsBtn,
                QIcon(":/img/SETTINGS.png"),
                iconDimension,
                maxButtonsWidth,
                "Settings");
    connect(mpSettingsBtn, &QPushButton::pressed, this, &StartPage::settingsBtnPressed);

    // bindong buttons
    QVBoxLayout *pBtnLayout = new QVBoxLayout;
    pBtnLayout->addWidget(mpNewBtn);
    pBtnLayout->addWidget(mpOpenBtn);
    pBtnLayout->addWidget(mpOpenDirBtn);
    pBtnLayout->addWidget(mpSettingsBtn);

    // creating & laying out labels
    QLabel *pNewLbl = new QLabel(tr("Create new file"));
    setupLabels(pNewLbl, lblFont);

    QLabel *pOpenLbl = new QLabel(tr("Open existing file"));
    setupLabels(pOpenLbl, lblFont);

    QLabel *pOpenDirLbl = new QLabel(tr("Open existing project directory"));
    setupLabels(pOpenDirLbl, lblFont);

    QLabel *pSettingsLbl = new QLabel(tr("Configure IDE"));
    setupLabels(pSettingsLbl, lblFont);    

    QVBoxLayout *pLblLayout = new QVBoxLayout;
    pLblLayout->addWidget(pNewLbl);
    pLblLayout->addWidget(pOpenLbl);
    pLblLayout->addWidget(pOpenDirLbl);
    pLblLayout->addWidget(pSettingsLbl);

    // window lay out
    QHBoxLayout *pWdwLayout = new QHBoxLayout;
    pWdwLayout->addLayout(pBtnLayout);
    pWdwLayout->addLayout(pLblLayout);

    setLayout(pWdwLayout);

    // receive current screen geometry
    QScreen *screen = qApp->screens().at(0);
    QRect screenGeometry = screen->geometry();

    // resizing & centring dialog
    resize(screenGeometry.width() / 3,
           screenGeometry.height() / 3);
    int x = screenGeometry.center().x() - width() / 2;
    int y = screenGeometry.center().y() - height() / 2;
    move(x, y);
}

void StartPage::showStartPage()
{
    exec();
}

void StartPage::setupButton(QPushButton *pButton,
                            QIcon icon, int iconDimension,
                            int maxWidth, const QString &text)
{
    pButton->setIcon(icon);
    pButton->setIconSize(QSize(iconDimension,iconDimension));
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

void StartPage::settingsBtnPressed()
{
    emit onSettingsBtnPressed();
    accept();
}

void StartPage::setupLabels(QLabel *pLabel,
                            QFont &lblFont)
{
    pLabel->setAlignment(Qt::AlignCenter);
    pLabel->setFont(lblFont);    
}
