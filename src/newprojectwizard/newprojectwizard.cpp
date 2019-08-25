#include "newprojectwizard.h"

#include <QApplication>
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

NewProjectDialog::NewProjectDialog(QWidget *pParent): QDialog (pParent)
{
    setWindowTitle("New Project");

    // info label
    QLabel *pLabel = new QLabel(userMessages[UserMessages::NewProjectWizardMsg]);
    pLabel->setWordWrap(true);
    QFont lblFont("Segoe UI", 12);
    lblFont.setBold(true);
    pLabel->setFont(lblFont);

    // project name label
    QLabel *pProjectNameLbl = new QLabel(tr("Project Name"));

    // project name lineEdit
    mpLine = new QLineEdit;
    mpLine->setText("Unnamed");
    QHBoxLayout *pLineEditLayout = new QHBoxLayout;
    pLineEditLayout->addWidget(pProjectNameLbl);
    pLineEditLayout->addWidget(mpLine);

    // laying out wgts
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addSpacing(50);
    pLayout->addWidget(pLabel);
    pLayout->addSpacing(50);
    pLayout->addLayout(pLineEditLayout);
    pLayout->addSpacing(20);

    // directory lineEdit
    mpDirLbl = new QLineEdit(QDir::homePath());
    mpDirLbl->setReadOnly(true);

    // browse button
    QPushButton *pBrowseBtn = new QPushButton(tr("Browse"));
    connect(pBrowseBtn, &QPushButton::clicked, this, &NewProjectDialog::onSelectDirectory);
    QHBoxLayout *pBrowseLayout = new QHBoxLayout;
    pBrowseLayout->addWidget(mpDirLbl);
    pBrowseLayout->addWidget(pBrowseBtn);
    pLayout->addLayout(pBrowseLayout);

    // create file & cancel buttons
    QPushButton *pOkBtn = new QPushButton(tr("Create Project"));
    QPushButton *pCancelBtn = new QPushButton(tr("Cancel"));
    connect(pOkBtn, &QPushButton::clicked, this, &NewProjectDialog::onCreateProject);
    connect(pCancelBtn, &QPushButton::clicked, this, &NewProjectDialog::reject);

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
           static_cast<int>(screenGeometry.height() * 0.4));
    int x = screenGeometry.center().x() - width() / 2;
    int y = screenGeometry.center().y() - height() / 2;
    move(x, y);

    // specify tab order
    setTabOrder(pOkBtn, pCancelBtn);
    setTabOrder(pCancelBtn, pBrowseBtn);
}

QString NewProjectDialog::start()
{
    // if user closes dialog
    if (!exec())
    {
        throw QException();
    }

    // full filename is returned to the caller
    return mProjectName;
}

bool NewProjectDialog::isValidDirName(const QString &dirName)
{
    return !dirName.contains(QRegExp(R"exp([*/:;|=,\\\[\]])exp"));
}

void NewProjectDialog::onSelectDirectory()
{
    mPath = QFileDialog::getExistingDirectory
            (this,
             userMessages[UserMessages::SelectDirectoryTitle],
             QDir::homePath());
    mpDirLbl->setText(mPath);
}

void NewProjectDialog::onCreateProject()
{    
    QString dirName = mpDirLbl->text();
    qDebug() << "dirName" << dirName;

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
        QMessageBox::warning(this, userMessages[UserMessages::WrongDirectoryTitle],
                             userMessages[UserMessages::EmptyFileNameMsg]);
        return;
    }
    if (!isValidDirName(mpLine->text()))
    {
        QMessageBox::warning(this, userMessages[UserMessages::InvalidDirectoryNameTitle],
                             userMessages[UserMessages::InvalidDirectoryNameMsg]);
        return;
    }

    QString dirPath = dirName + mpLine->text();
    qDebug() << "dirPath" << dirPath;

    // preventing overwriting existing files
    QDir dir(dirPath);

    qDebug() << "dirPath exists" << dir.exists();

    if (dir.exists())
    {
        QMessageBox::warning(this, userMessages[UserMessages::DirectoryAlreadyExistsTitle],
                             userMessages[UserMessages::DirectoryAlreadyExistsMsg]);
        return;
    }

    // creating new directory

    QDir newDir(dirName);

    if (!newDir.mkdir(mpLine->text()))
    {
        QMessageBox::warning(this, userMessages[UserMessages::ErrorTitle],
                userMessages[UserMessages::CreatingDirectoryFailureMsg]);
        return;
    }
    QMessageBox::information(this, userMessages[UserMessages::ProjectCreatedTitle],
            userMessages[UserMessages::ProjectCreatedMsg]);

    mProjectName = dirPath;

    accept();
}
