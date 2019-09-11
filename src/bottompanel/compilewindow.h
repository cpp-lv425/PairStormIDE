#ifndef COMPILEWINDOW_H
#define COMPILEWINDOW_H

#include <QWidget>
#include <codeeditor.h>
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
public slots:
    void setCompileOutput(QString);
private slots:
    void on_compileButton_clicked();

private:
    Ui::CompileWindow *ui;
    CodeEditor *compileEditor;
};

#endif // COMPILEWINDOW_H
