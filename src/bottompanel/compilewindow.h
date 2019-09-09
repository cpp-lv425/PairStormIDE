#ifndef COMPILEWINDOW_H
#define COMPILEWINDOW_H

#include <QWidget>

namespace Ui
{
class CompileWindow;
}

class CompileWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CompileWindow(QWidget *parent = nullptr);
    ~CompileWindow();
signals:
    void programIsReadyToCompile();

private slots:
    void on_compileButton_clicked();

    void on_Removeall_clicked();

private:
    Ui::CompileWindow *ui;
};

#endif // COMPILEWINDOW_H
