#include "newfilewizard.h"

#include <QApplication>
#include <QDirIterator>
#include <QVBoxLayout>
#include <QStringList>
#include <QListWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QScreen>
#include <QLabel>
#include <QDebug>
#include <QDir>

#include "usermessages.h"
#include "filemanager.h"
#include "utils.h"

NewFileDialog::NewFileDialog(QStringList &fileExtensions,
                             QString projectPath,
                             QWidget *pParent):
    QDialog (pParent),
    mProjectPath(projectPath)
{
    setWindowTitle("New File");

    // info label
    QLabel *pLabel = new QLabel(userMessages[UserMessages::NewFileWizardMsg]);
    pLabel->setWordWrap(true);
    QFont lblFont("Segoe UI", 12);
    lblFont.setBold(true);
    pLabel->setFont(lblFont);    

    // filename label
    QLabel *pFileNameLbl = new QLabel(tr("File Name"));

    // filename lineEdit
    mpLine = new QLineEdit;
    mpLine->setText("Unnamed");
    QHBoxLayout *pLineEditLayout = new QHBoxLayout;
    pLineEditLayout->addWidget(pFileNameLbl);
    pLineEditLayout->addWidget(mpLine);

    // file extensions list
    mpExtensionsList = new QListWidget;
    for(const auto& item: fileExtensions)
    {
        mpExtensionsList->addItem(item);
    }
    mpExtensionsList->setCurrentRow(0);

    // laying out wgts
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addSpacing(50);
    pLayout->addWidget(pLabel);
    pLayout->addSpacing(50);
    pLayout->addLayout(pLineEditLayout);
    pLayout->addSpacing(20);
    pLayout->addWidget(mpExtensionsList);

    // directory lineEdit
    mpDirLbl = new QLineEdit(projectPath);
    mpDirLbl->setReadOnly(true);

    // browse button
    QPushButton *pBrowseBtn = new QPushButton(tr("Browse"));
    connect(pBrowseBtn, &QPushButton::clicked, this, &NewFileDialog::onSelectDirectory);
    QHBoxLayout *pBrowseLayout = new QHBoxLayout;
    pBrowseLayout->addWidget(mpDirLbl);
    pBrowseLayout->addWidget(pBrowseBtn);
    pLayout->addLayout(pBrowseLayout);

    // create file & cancel buttons
    QPushButton *pOkBtn = new QPushButton(tr("Create File"));
    QPushButton *pCancelBtn = new QPushButton(tr("Cancel"));
    connect(pOkBtn, &QPushButton::clicked, this, &NewFileDialog::onCreateFile);
    connect(pCancelBtn, &QPushButton::clicked, this, &NewFileDialog::reject);

    // laying out wgts
    QHBoxLayout *pStdBtnLayout = new QHBoxLayout;
    pStdBtnLayout->addStretch(1);
    pStdBtnLayout->addWidget(pOkBtn);
    pStdBtnLayout->addWidget(pCancelBtn);
    pLayout->addSpacing(50);
    pLayout->addLayout(pStdBtnLayout);

    setLayout(pLayout);

    // receive current screen geometry
    QScreen *screen = qApp->screens().at(0);
    QRect screenGeometry = screen->geometry();

    // resizing & centring dialog
    resize(screenGeometry.width() / 2,
           static_cast<int>(screenGeometry.height() * 0.6));
    int x = screenGeometry.center().x() - width() / 2;
    int y = screenGeometry.center().y() - height() / 2;
    move(x, y);

    // specify tab order
    setTabOrder(pOkBtn, pCancelBtn);
    setTabOrder(pCancelBtn, pBrowseBtn);
}

QString NewFileDialog::start()
{
    // if user closes dialog
    if (!exec())
    {
        throw QException();
    }

    // full filename is returned to the caller
    return mFileName;
}

bool NewFileDialog::isValidFilename(const QString &fileName)
{
    return !fileName.contains(QRegExp(invalidFileOrDirNameRegex));
}

bool NewFileDialog::directoryBelongsToProject(QString dirPath)
{
    if (mProjectPath == dirPath)
    {
        return true;
    }
    QDirIterator dirIter(mProjectPath, QDir::Dirs, QDirIterator::Subdirectories);

    while (dirIter.hasNext())
    {
        dirIter.next();
        if (dirIter.filePath() == dirPath)
        {
            return true;
        }
    }
    return false;
}

void NewFileDialog::onSelectDirectory()
{
    QString path = QFileDialog::getExistingDirectory
            (this,
             userMessages[UserMessages::SelectDirectoryTitle],
             mProjectPath);
    mpDirLbl->setText(path);
}

void NewFileDialog::onCreateFile()
{
    QString fileName = mpLine->text() + mpExtensionsList->currentItem()->text();
    QString dirName = mpDirLbl->text();

    // check if directory is not empty
    if (dirName.isEmpty())
    {
        QMessageBox::warning(this, userMessages[UserMessages::WrongDirectoryTitle],
                             userMessages[UserMessages::WrongDirectoryMsg]);
        return;
    }

    // if file name is empty string
    if (mpLine->text().isEmpty())
    {
        QMessageBox::warning(this, userMessages[UserMessages::WrongFileNameTitle],
                             userMessages[UserMessages::EmptyFileNameMsg]);
        return;
    }

    if (!isValidFilename(mpLine->text()))
    {
        QMessageBox::warning(this, userMessages[UserMessages::WrongFileNameTitle],
                             userMessages[UserMessages::InvalidFileNameMsg]);
        return;
    }

    if (!directoryBelongsToProject(dirName))
    {
        QMessageBox::warning(this,
                             userMessages[UserMessages::WrongDirectoryTitle],
                             userMessages[UserMessages::DirectoryDoesNotBelongToProjectMsg]);
        return;
    }

    // preventing overwriting existing files
    QDir dir(dirName);
    if (dir.exists(fileName))
    {
        QMessageBox::warning(this, userMessages[UserMessages::FileAlreadyExistsTitle],
                             userMessages[UserMessages::FileAlreadyExistsMsg]);
        return;
    }

    // creating new file
    try
    {
        FileManager().createFile(dirName + '/' + fileName);
    } catch (const FileOpeningFailure&)
    {
        QMessageBox::warning(this, userMessages[UserMessages::ErrorTitle],
                userMessages[UserMessages::CreatingFileFailureMsg]);
        return;
    }
    QMessageBox::information(this, userMessages[UserMessages::FileCreatedTitle],
            userMessages[UserMessages::FileCreatedMsg]);

    mFileName = dirName + '/' + fileName;

    accept();
}
