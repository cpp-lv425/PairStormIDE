#ifndef CHOICEWINDOW_H
#define CHOICEWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QListWidget;
class QAbstractButton;
class QListWidgetItem;
QT_END_NAMESPACE

class ChoiceWindow : public QDialog
{
    Q_OBJECT
public:
    ChoiceWindow(QStringList &usersFilesList, QWidget *pParent = nullptr);

private:
    QLabel *mpLabelAccounts;
    //QStringList mUsersList;

    //QPushButton *mpNewUserButton;
    //QPushButton *mpTryModeButton;
    QListWidget *mpUsersWidgetList;

    QPushButton *mpUnnamedUser;
    QPushButton *mpNewUser;

    QDialogButtonBox *mpButtonBox;

    // design parameters
    //int mColumnSpasing              = 10;
    //int mStretchFactor              = 20;
    //int mStretchFactorMain          = 100;
    //int mCompilerWidth              = 100;
    int mBasicStretch               = 1;
private slots:
    void onListDoubleClicked(QListWidgetItem * item);
    void onNewUserClicked();
    void onUnnamedUserClicked();
    void onBtnBoxClicked(QAbstractButton * button);
signals:
    void cancel();
    void choice(QString userName);
};

#endif // CHOICEWINDOW_H
