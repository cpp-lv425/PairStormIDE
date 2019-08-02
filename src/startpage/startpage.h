#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QDialog>

namespace Ui {
class StartPage;
}

class QLabel;

class StartPage : public QDialog
{
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);
    void showStartPage();    
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

private:
    void setupLabels(QLabel *pLabel,
                     QFont &lblFont,
                     QPalette &palette);
    void setupButton(QPushButton *pButton,
                     QIcon icon,
                     int maxWidth);
};

#endif // STARTPAGE_H
