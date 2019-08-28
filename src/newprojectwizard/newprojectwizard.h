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
    QString promptProjectNameFromUser();

private:
    QLineEdit *mpLine;
    QString mPath;
    QString mProjectName;
    QLineEdit *mpDirLbl;

    bool isValidProjectName(const QString& dirName);

private slots:
    void onSelectDirectory();
    void onCreateProject();
};

#endif // NEWPROJECTWIZARD_H
