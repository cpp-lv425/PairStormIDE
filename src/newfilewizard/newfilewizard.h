#ifndef NEWFILEWIZARD_H
#define NEWFILEWIZARD_H

#include <QWizardPage>
#include <QWizard>

class QListWidget;
class QLineEdit;
class QLabel;

class NewFileWizard: public QWizard
{
    Q_OBJECT

public:
    NewFileWizard(QStringList &fileExtensions,
                  QWidget *pParent = nullptr);
    virtual void accept() override;

private:
    QStringList mFileExtensions;
};

class IntroPage: public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget *pParent = nullptr);
};

class FileInfoPage: public QWizardPage
{
    Q_OBJECT
public:
    FileInfoPage(QStringList &fileExtensions,
                 QWidget *pParent = nullptr);

private:
    QLineEdit *mpLine;
    QListWidget *mpExtensionsList;
    QString mPath;
    QLineEdit *pDirLbl;

private slots:
    void onSelectDirectory();
};

#endif // NEWFILEWIZARD_H
