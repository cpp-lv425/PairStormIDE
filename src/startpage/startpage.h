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
    QPushButton *mpNewBtn;
    QPushButton *mpOpenBtn;
    QPushButton *mpOpenDirBtn;
    QPushButton *mpReferenceBtn;
    QPushButton *mpSettingsBtn;

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
                     int maxWidth, const QString& text);
};

#endif // STARTPAGE_H
