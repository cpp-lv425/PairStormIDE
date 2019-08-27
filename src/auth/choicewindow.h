#ifndef CHOICEWINDOW_H
#define CHOICEWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
class QAbstractButton;
class QListWidgetItem;
class QDialogButtonBox;
QT_END_NAMESPACE

class ChoiceWindow : public QDialog
{
    Q_OBJECT
public:
    ChoiceWindow(const QStringList &usersFilesList, QWidget *pParent = nullptr);

private:
    QListWidget *mpUsersWidgetList;

    QPushButton *mpUnnamedUserButton;
    QPushButton *mpNewUserButton;

    QDialogButtonBox *mpButtonBox;

    const QString mTitle {"Sign in, please"};

    // design parameters
    int mBasicStretch               = 1;

private slots:
    void onListDoubleClicked(QListWidgetItem *item);
    void onNewUserClicked();
    void onUnnamedUserClicked();
    void onBtnBoxClicked(QAbstractButton *button);

signals:
    void cancel();
    void choice(QString userName);

};

#endif // CHOICEWINDOW_H
