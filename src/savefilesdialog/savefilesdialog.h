#ifndef SAVEFILESDIALOG_H
#define SAVEFILESDIALOG_H

#include <QDialogButtonBox>
#include <QDialog>

class QAbstractButton;

class SaveFilesDialog: public QDialog
{
    Q_OBJECT

public:
    SaveFilesDialog(const QStringList &unsavedFiles,
                    QWidget *pParent = nullptr);
    QDialogButtonBox::StandardButton start();

private slots:
    void onClicked(QAbstractButton* pButton);

private:
    QDialogButtonBox *pButtonBox;
    QDialogButtonBox::StandardButton pressedButton;
};

#endif // SAVEFILESDIALOG_H
