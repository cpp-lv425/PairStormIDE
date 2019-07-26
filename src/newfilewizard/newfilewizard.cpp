#include "newfilewizard.h"

#include <QVBoxLayout>
#include <QStringList>
#include <QListWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QDir>

NewFileWizard::NewFileWizard(QStringList &fileExtensions,
                             QWidget *pParent):
    mFileExtensions(fileExtensions)
{
    addPage(new IntroPage);
    addPage(new FileInfoPage(mFileExtensions));
}

void NewFileWizard::accept()
{
    QString fileName = field("fileName").toString();
    int fileExtension = field("extension").toInt();
    QString dir = field("dir").toString();
    qDebug() << fileName << mFileExtensions[fileExtension];
    qDebug() << dir;
}

IntroPage::IntroPage(QWidget *pParent): QWizardPage (pParent)
{
    setTitle("Introduction");

    QLabel *pLabel = new QLabel("This wizard will help you create new file.");
    pLabel->setWordWrap(true);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(pLabel);
    setLayout(pLayout);
}

FileInfoPage::FileInfoPage(QStringList &fileExtensions, QWidget *pParent): QWizardPage (pParent)
{
    setTitle("File information");

    QLabel *pLabel = new QLabel("Please specify file name, extension & project directory.");
    pLabel->setWordWrap(true);

    QLabel *pFileNameLbl = new QLabel(tr("File Name"));
    mpLine = new QLineEdit;
    QHBoxLayout *pLineEditLayout = new QHBoxLayout;
    pLineEditLayout->addWidget(pFileNameLbl);
    pLineEditLayout->addWidget(mpLine);

    mpExtensionsList = new QListWidget;
    for(const auto& item: fileExtensions)
        mpExtensionsList->addItem(item);
    mpExtensionsList->setCurrentRow(0);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addLayout(pLineEditLayout);
    pLayout->addSpacing(20);
    pLayout->addWidget(mpExtensionsList);

    pDirLbl = new QLineEdit(QDir::homePath());
    pDirLbl->setReadOnly(true);
    QPushButton *pBrowseBtn = new QPushButton(tr("Browse"));
    connect(pBrowseBtn, &QPushButton::clicked, this, &FileInfoPage::onSelectDirectory);
    QHBoxLayout *pBrowseLayout = new QHBoxLayout;
    pBrowseLayout->addWidget(pDirLbl);
    //pBrowseLayout->addStretch(1);
    pBrowseLayout->addWidget(pBrowseBtn);

    pLayout->addLayout(pBrowseLayout);

    registerField("fileName", mpLine);
    registerField("extension", mpExtensionsList);
    registerField("dir", pDirLbl);

    setLayout(pLayout);
}

void FileInfoPage::onSelectDirectory()
{
    mPath = QFileDialog::getExistingDirectory(this, "Select project directory", QDir::homePath());
    pDirLbl->setText(mPath);
}
