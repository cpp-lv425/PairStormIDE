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
    void setCompileErrorsOutput(QString);
    void setCompileSuccessOutput();
    void clearCompileOutputView();
private:
    QStringList getAllErrorsFromCompileOutput(const QString &compileErrorsOutput);
    QString removeAllSymbolsFromString( QString &outputLine, const char &symb);
private:
    Ui::CompileWindow *ui;
    QListWidget *mpCompileOutputList;
};

#endif // COMPILEWINDOW_H
