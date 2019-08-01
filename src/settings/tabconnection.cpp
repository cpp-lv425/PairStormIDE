#include "tabconnection.h"

#include <QtWidgets>

TabConnection::TabConnection(QWidget *parent)
    : QWidget(parent)
{
    QLabel *fileNameLabel = new QLabel("Connection fields");
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fileNameLabel);

    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
