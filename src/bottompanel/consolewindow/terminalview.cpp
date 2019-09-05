#include "terminalview.h"
#include "ui_terminalview.h"

TerminalView::TerminalView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerminalView)
{
    ui->setupUi(this);
}

TerminalView::~TerminalView()
{
    delete ui;
}

QPlainTextEdit* TerminalView::getTerminalTextEdit()
{
   return ui->inputCommandsEdit;
}
