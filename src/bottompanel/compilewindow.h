#ifndef COMPILEWINDOW_H
#define COMPILEWINDOW_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QStringList>
#include <QListWidget>

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
    QStringList getAllErrorsFromCompileOutput(const QString &compileErrorsOutput);
private:
    Ui::CompileWindow *ui;
    QListWidget *compileOutputList;
};

#endif // COMPILEWINDOW_H
