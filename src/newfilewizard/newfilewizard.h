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
                 QWidget *pParent = nullptr);
    QString start();
    bool isValidFilename(const QString& fileName);

private:
    QLineEdit *mpLine;
    QListWidget *mpExtensionsList;
    QString mPath;
    QString mFileName;
    QLineEdit *mpDirLbl;

private slots:
    void onSelectDirectory();
    void onCreateFile();    
};

#endif // NEWFILEWIZARD_H
