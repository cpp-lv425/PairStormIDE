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

    QListWidget *pStartPageList;
public:
    explicit StartPage(QWidget *parent = nullptr);
    ~StartPage();

private:
    Ui::StartPage *ui;
};

#endif // STARTPAGE_H
