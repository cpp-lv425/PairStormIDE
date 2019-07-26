#include "chatwidget.h"

#include <QPlainTextEdit>
#include <QBoxLayout>
#include <QLineEdit>
#include <QLabel>

ChatWidget::ChatWidget(QWidget *pParent): QWidget (pParent)
{
    pFeed = new QPlainTextEdit;
    QLabel *pLbl = new QLabel(">> ");
    pEnterLine = new QLineEdit;
    QHBoxLayout *pLineLayout = new QHBoxLayout;
    pLineLayout->addWidget(pLbl);
    pLineLayout->addWidget(pEnterLine);
    QVBoxLayout *pWindowLayout = new QVBoxLayout;
    pWindowLayout->addWidget(pFeed);
    pWindowLayout->addLayout(pLineLayout);
    setLayout(pWindowLayout);
}
