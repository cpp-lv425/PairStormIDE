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
#include <QDir>

#include "filemanager.h"

NewFileDialog::NewFileDialog(QStringList &fileExtensions,
                             QWidget *pParent): QDialog (pParent)
{
    setWindowTitle("New File");

    // info label
    QLabel *pLabel = new QLabel(tr("Please specify file name, file extension and project directory."));
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
    if (!exec())
    {
        throw QException();
    }

    // full filename is returned to the caller
    return mFileName;
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

    // check if directory is not empty
    if (dirName.isEmpty())
    {
        QMessageBox::warning(this, "Wrong directory",
                             "Please specify correct file directory.");
        return;
    }

    // if file name is empty string
    if(mpLine->text().isEmpty())
    {
        QMessageBox::warning(this, "Incorrect name",
                             "Please enter file name.");
        return;
    }

    // preventing overwriting existing files
    QDir dir(dirName);
    if (dir.exists(fileName))
    {
        QMessageBox::warning(this, "File exists",
                             "File with specified name already exists. "
                             "Please select other file name.");
        return;
    }

    // creating new file
    try
    {
        FileManager().createFile(dirName + '/' + fileName);
    } catch (const QException&)
    {
        QMessageBox::warning(this, "Error", "Unable to create file");
        return;
    }
    QMessageBox::information(this, "File Created", "Specified file has been successfully created.");

    // check if filename is an empty string
    if (fileName.isEmpty())
    {
        mFileName = QString();
    }
    else
    {
        mFileName = dirName + '/' + fileName;
    }
    accept();
}
