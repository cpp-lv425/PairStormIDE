#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QDialog>

class QListWidget;

namespace Ui {
class StartPage;
}

class StartPage : public QDialog
{
    Q_OBJECT

    QListWidget *mpStartPageList;
public:
    explicit StartPage(QWidget *parent = nullptr);
    ~StartPage();

private:
    Ui::StartPage *ui;
    QPushButton *pNewBtn;
    QPushButton *pOpenBtn;
    QPushButton *pOpenDirBtn;
    QPushButton *pReferenceBtn;
    QPushButton *pSettingsBtn;

private slots:
    void newBtnPressed();
    void openBtnPressed();
    void openDirPressed();
    void referenceBtnPressed();
    void settingsBtnPressed();

signals:
    void onNewBtnPressed();
    void onOpenBtnPressed();
    void onOpenDirPressed();
    void onReferenceBtnPressed();
    void onSettingsBtnPressed();
};

#endif // STARTPAGE_H
