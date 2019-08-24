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
    QPushButton *mpNewProjectBtn;
    QPushButton *mpOpenProjectBtn;
    QPushButton *mpSettingsBtn;

private slots:
    void newProjectBtnPressed();
    void openProjectBtnPressed();
    void settingsBtnPressed();

signals:
    void onNewProjectBtnPressed();
    void onOpenProjectBtnPressed();
    void onSettingsBtnPressed();

private:
    void setupLabels(QLabel *pLabel,
                     QFont &lblFont);
    void setupButton(QPushButton *pButton,
                     QIcon icon,
                     int iconDimension,
                     int maxWidth, const QString& text);
};

#endif // STARTPAGE_H
