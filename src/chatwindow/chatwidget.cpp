#include "chatwidget.h"

#include <QPlainTextEdit>
#include <QListWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QLabel>

ChatWidget::ChatWidget(QWidget *pParent): QWidget (pParent)
{
    mpUsersList = new QListWidget;
    mpUsersList->addItem("Volodya");

    mpFeed = new QPlainTextEdit;
    QLabel *pLbl = new QLabel(">> ");
    mpEnterLine = new QLineEdit;
    QHBoxLayout *pLineLayout = new QHBoxLayout;
    pLineLayout->addWidget(pLbl);
    pLineLayout->addWidget(mpEnterLine);
    QVBoxLayout *pWindowLayout = new QVBoxLayout;
    pWindowLayout->addWidget(mpFeed);
    pWindowLayout->addLayout(pLineLayout);
    setLayout(pWindowLayout);
}
