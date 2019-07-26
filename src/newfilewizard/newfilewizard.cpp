#include "newfilewizard.h"

#include <QVBoxLayout>
#include <QStringList>
#include <QListWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QException>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QDir>

#include "filemanager.h"

NewFileDialog::NewFileDialog(QStringList &fileExtensions,
                             QWidget *pParent): QDialog (pParent)
{
    setWindowTitle("New File");

    QLabel *pLabel = new QLabel(tr("Please specify file name, file extension and project directory."));
    pLabel->setWordWrap(true);
    QFont lblFont("Segoe UI", 12);
    lblFont.setBold(true);
    pLabel->setFont(lblFont);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::blue);
    pLabel->setPalette(palette);

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
    pLayout->addSpacing(50);
    pLayout->addWidget(pLabel);
    pLayout->addSpacing(50);
    pLayout->addLayout(pLineEditLayout);
    pLayout->addSpacing(20);
    pLayout->addWidget(mpExtensionsList);

    mpDirLbl = new QLineEdit(QDir::homePath());
    mpDirLbl->setReadOnly(true);
    QPushButton *pBrowseBtn = new QPushButton(tr("Browse"));
    connect(pBrowseBtn, &QPushButton::clicked, this, &NewFileDialog::onSelectDirectory);
    QHBoxLayout *pBrowseLayout = new QHBoxLayout;
    pBrowseLayout->addWidget(mpDirLbl);
    pBrowseLayout->addWidget(pBrowseBtn);

    pLayout->addLayout(pBrowseLayout);

    QPushButton *pOkBtn = new QPushButton(tr("Create File"));
    QPushButton *pCancelBtn = new QPushButton(tr("Cancel"));
    connect(pOkBtn, &QPushButton::clicked, this, &NewFileDialog::onCreateFile);
    connect(pCancelBtn, &QPushButton::clicked, this, &NewFileDialog::onCancel);

    QHBoxLayout *pStdBtnLayout = new QHBoxLayout;
    pStdBtnLayout->addStretch(1);
    pStdBtnLayout->addWidget(pOkBtn);
    pStdBtnLayout->addWidget(pCancelBtn);
    pLayout->addSpacing(50);
    pLayout->addLayout(pStdBtnLayout);

    setLayout(pLayout);

    setGeometry(geometry().x(), geometry().y(), 700, 500);

    QPoint cntr = pParent->geometry().center();
    int x = cntr.x() - width() / 2;
    int y = cntr.y() - height() / 2;
    move(x, y);
}

void NewFileDialog::onSelectDirectory()
{
    mPath = QFileDialog::getExistingDirectory(this, "Select project directory", QDir::homePath());
    mpDirLbl->setText(mPath);
}

void NewFileDialog::onCreateFile()
{
    QString fileName = mpLine->text() + mpExtensionsList->currentItem()->text();
    QString dirName = mpDirLbl->text();

    QDir dir(dirName);

    if(dir.exists(fileName))
    {
        QMessageBox::warning(this, "File exists",
                             "File with specified name already exists. "
                             "Please select other file name.");
        return;
    }
    try
    {
        FileManager().createFile(dirName + '/' + fileName);
    } catch (const QException&)
    {
        QMessageBox::warning(this, "Error", "Unable to create file");
        return;
    }
    QMessageBox::information(this, "File Created", "Specified file has been successfully created.");
    accept();
}

void NewFileDialog::onCancel()
{
    reject();
}
