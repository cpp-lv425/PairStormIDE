#ifndef TERMINALVIEW_H
#define TERMINALVIEW_H

#include <QWidget>
#include <QString>
#include <QPlainTextEdit>

namespace Ui
{
class TerminalView;
}

class TerminalView : public QWidget
{
    Q_OBJECT

public:
    explicit TerminalView(QWidget *parent = nullptr);
    ~TerminalView();
    QPlainTextEdit* getTerminalTextEdit();
private:
    Ui::TerminalView *ui;
};

#endif // TERMINALVIEW_H
