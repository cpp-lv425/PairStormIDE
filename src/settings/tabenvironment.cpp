#include "tabenvironment.h"

#include <QtWidgets>

TabEnvironment::TabEnvironment(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch(1);

    //  label with configuration file path'
    QLabel *label = new QLabel("Configuration file:");
    mainLayout->addWidget(label);

    currentPath = QDir::currentPath();
    currentPath += "/conf.json";
    QFile file(currentPath);
    QLabel *pathLabel {file.exists() ? new QLabel(currentPath) : new QLabel("  ... not found.")};
    mainLayout->addWidget(pathLabel);

    //  modify configuration file
    QPushButton *pBtn = new QPushButton("Modify ...", this);
    pBtn->setMinimumWidth(100);
    pBtn->setMaximumWidth(100);
    mainLayout->addWidget(pBtn);
    pBtn->setEnabled(file.exists());
    connect(pBtn, &QPushButton::clicked, this, &TabEnvironment::onClicked);

    setLayout(mainLayout);
}//connect(buttonBox, &QDialogButtonBox::clicked, this, &MenuOptions::onBtnClicked);

void TabEnvironment::onClicked()
{
    QDesktopServices::openUrl(QUrl(currentPath));
}

