#ifndef NEWPROJECTWIZARD_H
#define NEWPROJECTWIZARD_H

#include <QDialog>
#include <QMap>

class QListWidget;
class QLineEdit;
class QLabel;

class NewProjectDialog: public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *pParent = nullptr);
    QString start();

private:
    QLineEdit *mpLine;
    QString mPath;
    QString mProjectName;
    QLineEdit *mpDirLbl;

    bool isValidDirName(const QString& dirName);

private slots:
    void onSelectDirectory();
    void onCreateProject();
};

#endif // NEWPROJECTWIZARD_H
