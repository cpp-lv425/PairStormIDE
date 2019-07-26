#ifndef NEWFILEWIZARD_H
#define NEWFILEWIZARD_H

#include <QDialog>

class QListWidget;
class QLineEdit;
class QLabel;

class NewFileDialog: public QDialog
{
    Q_OBJECT
public:
    NewFileDialog(QStringList &fileExtensions,
                 QWidget *pParent = nullptr);

private:
    QLineEdit *mpLine;
    QListWidget *mpExtensionsList;
    QString mPath;
    QLineEdit *mpDirLbl;

private slots:
    void onSelectDirectory();
    void onCreateFile();
    void onCancel();
};

#endif // NEWFILEWIZARD_H
