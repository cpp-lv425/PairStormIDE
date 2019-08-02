#include "newfilewizard.h"

#include <QVBoxLayout>
#include <QStringList>
#include <QListWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QDir>

#include "usermessages.h"
#include "filemanager.h"
#include "utils.h"

NewFileDialog::NewFileDialog(QStringList &fileExtensions,
                             QWidget *pParent): QDialog (pParent)
{
    setWindowTitle("New File");

    // info label
    QLabel *pLabel = new QLabel(userMessages[UserMessages::NewFileWizardMsg]);
    pLabel->setWordWrap(true);
    QFont lblFont("Segoe UI", 12);
    lblFont.setBold(true);
    pLabel->setFont(lblFont);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::blue);
    pLabel->setPalette(palette);

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
    mpDirLbl = new QLineEdit(QDir::homePath());
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

    // specifying initial dialog size
    setGeometry(geometry().x(), geometry().y(), 700, 500);

    // centring dialog
    QPoint cntr = pParent->geometry().center();
    int x = cntr.x() - width() / 2;
    int y = cntr.y() - height() / 2;
    move(x, y);
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
    return !fileName.contains(QRegExp(R"exp([*/:;|=,\\\[\]])exp"));
}

void NewFileDialog::onSelectDirectory()
{
    mPath = QFileDialog::getExistingDirectory
            (this,
             userMessages[UserMessages::SelectDirectoryTitle],
             QDir::homePath());
    mpDirLbl->setText(mPath);
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
