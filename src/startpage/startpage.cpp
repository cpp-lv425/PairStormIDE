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
    const int iconDimension = 50;

    QFont lblFont("Segoe UI", 12);
    lblFont.setBold(true);

    // creating & configuring new file button
    mpNewProjectBtn = new QPushButton;
    setupButton(mpNewProjectBtn,
                QIcon(":/img/NEWPROJECT.png"),
                iconDimension);
    connect(mpNewProjectBtn, &QPushButton::pressed, this, &StartPage::newProjectBtnPressed);

    // creating & configuring open folder button
    mpOpenProjectBtn = new QPushButton;
    setupButton(mpOpenProjectBtn,
                QIcon(":/img/OPENDIR.png"),
                iconDimension);
    connect(mpOpenProjectBtn, &QPushButton::pressed, this, &StartPage::openProjectBtnPressed);

    // creating & configuring settings call button
    mpSettingsBtn = new QPushButton;
    setupButton(mpSettingsBtn,
                QIcon(":/img/SETTINGS.png"),
                iconDimension);
    connect(mpSettingsBtn, &QPushButton::pressed, this, &StartPage::settingsBtnPressed);

    // bindong buttons
    QVBoxLayout *pBtnLayout = new QVBoxLayout;
    pBtnLayout->addWidget(mpNewProjectBtn);    
    pBtnLayout->addWidget(mpOpenProjectBtn);
    pBtnLayout->addWidget(mpSettingsBtn);

    // creating & laying out labels
    QLabel *pNewLbl = new QLabel(tr("Create new project"));
    setupLabels(pNewLbl, lblFont);    

    QLabel *pOpenDirLbl = new QLabel(tr("Open existing project"));
    setupLabels(pOpenDirLbl, lblFont);

    QLabel *pSettingsLbl = new QLabel(tr("Configure IDE"));
    setupLabels(pSettingsLbl, lblFont);    

    QVBoxLayout *pLblLayout = new QVBoxLayout;
    pLblLayout->addWidget(pNewLbl);
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
                            QIcon icon, int iconDimension)
{
    pButton->setIcon(icon);
    pButton->setIconSize(QSize(iconDimension, iconDimension));
    pButton->setSizePolicy(QSizePolicy::Minimum,
                           QSizePolicy::Minimum);
}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::newProjectBtnPressed()
{
    emit onNewProjectBtnPressed();
    accept();
}

void StartPage::openProjectBtnPressed()
{
    emit onOpenProjectBtnPressed();
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
    pLabel->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::Expanding);
    pLabel->setFont(lblFont);    
}
