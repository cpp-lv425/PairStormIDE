#ifndef NEWFILEWIZARD_H
#define NEWFILEWIZARD_H

#include <QDialog>
#include <QMap>

class QListWidget;
class QLineEdit;
class QLabel;

class NewFileDialog: public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDialog(QStringList &fileExtensions,
                           QString projectPath,
                           QWidget *pParent = nullptr);
    QString start();

private:
    QLineEdit *mpLine;
    QListWidget *mpExtensionsList;
    QString mProjectPath;
    QString mFileName;
    QLineEdit *mpDirLbl;

    bool isValidFilename(const QString &fileName);
    bool directoryBelongsToProject(QString dirPath);

private slots:
    void onSelectDirectory();
    void onCreateFile();
};

#endif // NEWFILEWIZARD_H
