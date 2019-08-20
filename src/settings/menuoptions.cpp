#include "menuoptions.h"

#include "tabenvironment.h"
#include "tabtexteditor.h"
#include "tabconnection.h"
#include "tabcpp.h"
#include "tabrepository.h"

#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QSettings>
#include <QDialog>
#include <QDebug>

MenuOptions::MenuOptions(QWidget *pParent) : QDialog (pParent)
{
    setModal(true);
    setMinimumWidth(mMenuWidth);
    setMaximumWidth(mMenuWidth);
    setMinimumHeight(mMenuHeight);
    setMaximumHeight(mMenuHeight);
    setWindowTitle("Settings");

    mpTabWidget = new QTabWidget(this);

    mpTabEnvironment = new TabEnvironment(this);
    mpTabTextEditor = new TabTextEditor(this);
    mpTabCpp = new TabCpp(this);
    mpTabRepository = new TabRepository(this);
    mpTabConnection = new TabConnection(this);


    mpTabWidget->addTab(mpTabEnvironment, "Environment");
    mpTabWidget->addTab(mpTabTextEditor, "Text Editor");
    mpTabWidget->addTab(mpTabCpp, "C++");
    mpTabWidget->addTab(mpTabRepository, "Repository");
    mpTabWidget->addTab(mpTabConnection, "Connection");

    mpButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
           | QDialogButtonBox::Apply | QDialogButtonBox::Cancel, this);

    connect(mpButtonBox, &QDialogButtonBox::clicked, this, &MenuOptions::onBtnClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mpTabWidget);
    mainLayout->addWidget(mpButtonBox);
    setLayout(mainLayout);
    show();
}

void MenuOptions::onBtnApplyClicked()
{
    //      APPLY ALL CHANGES IN TABS
    QSettings s;
    // tab Environment
    if(mpTabEnvironment->getStyleCurrernt() != mpTabEnvironment->getStyleNew())
    {
        mpTabEnvironment->setStyleCurrernt(mpTabEnvironment->getStyleNew());    // change current value to new
        s.setValue("style", mpTabEnvironment->getStyleCurrernt());    // apply change in QSettings
        mNewValuesToChange["style"] = mpTabEnvironment->getStyleCurrernt();   // add change to emit to uplevel
    }
    // tab Text Editor - font size
    if(mpTabTextEditor->getFontSizeCurrernt() != mpTabTextEditor->getFontSizeNew())
    {
        mpTabTextEditor->setFontSizeCurrernt(mpTabTextEditor->getFontSizeNew()); // change current value to new
        s.setValue("editorFontSize", mpTabTextEditor->getFontSizeCurrernt());    // apply change in QSettings
        mNewValuesToChange["editorFontSize"] = mpTabTextEditor->getFontSizeCurrernt(); // add change to emit to uplevel
    }
    // tab Text Editor - font name
    if(mpTabTextEditor->getFontCurrernt() != mpTabTextEditor->getFontNew())
    {
        mpTabTextEditor->setFontCurrernt(mpTabTextEditor->getFontNew());    // change current value to new
        s.setValue("editorFontName", mpTabTextEditor->getFontCurrernt());    // apply change in QSettings
        mNewValuesToChange["editorFont"] = mpTabTextEditor->getFontCurrernt(); // add change to emit to uplevel
    }
    // tab C++ - standart
    if(mpTabCpp->getStandartCurrernt() != mpTabCpp->getStandartNew())
    {
        mpTabCpp->setStandartCurrernt(mpTabCpp->getStandartNew());    // change current value to new
        s.setValue("cppStandart", mpTabCpp->getStandartCurrernt());    // apply change in QSettings
        mNewValuesToChange["cppStandart"] = mpTabCpp->getStandartCurrernt(); // add change to emit to uplevel
    }

    // tab Connection - UDP port
    if(mpTabConnection->getUdpCurrernt() != mpTabConnection->getUdpNew() && mpTabConnection->getUdpNew().size())
    {
        mpTabConnection->setUdpCurrernt(mpTabConnection->getUdpNew());    // change current value to new
        s.setValue("defaultUdpPortNumber", mpTabConnection->getUdpNew());    // apply change in QSettings
        mNewValuesToChange["UdpPortNumber"] = mpTabConnection->getUdpNew(); // add change to emit to uplevel
    }
    // tab Connection - TCP port
    if(mpTabConnection->getTcpCurrernt() != mpTabConnection->getTcpNew() && mpTabConnection->getTcpNew().size())
    {
        mpTabConnection->setTcpCurrernt(mpTabConnection->getTcpNew());    // change current value to new
        s.setValue("defaultTcpPortNumber", mpTabConnection->getTcpNew());    // apply change in QSettings
        mNewValuesToChange["TcpPortNumber"] = mpTabConnection->getTcpNew(); // add change to emit to uplevel
    }

    if (mNewValuesToChange.size())
    {
        emit valuesChanged(mNewValuesToChange);
    }
}

void MenuOptions::onBtnClicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton pressedButton;
    pressedButton = mpButtonBox->standardButton(button);

    if (pressedButton == QDialogButtonBox::Cancel)
    {
        close();
    }
    if (pressedButton == QDialogButtonBox::Apply)
    {
        onBtnApplyClicked();
    }
    if (pressedButton == QDialogButtonBox::Ok)
    {
        onBtnApplyClicked();
        close();
    }
}


