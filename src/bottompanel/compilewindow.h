#ifndef COMPILEWINDOW_H
#define COMPILEWINDOW_H

#include <QWidget>
#include <QPlainTextEdit>
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

private:
    Ui::CompileWindow *ui;
    QPlainTextEdit *compileOutputEditor;
};

#endif // COMPILEWINDOW_H
