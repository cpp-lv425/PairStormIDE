#include "tabcpp.h"

#include <QtWidgets>

TabCpp::TabCpp(QWidget *parent)
    : QWidget(parent)
{
    QLabel *fileNameLabel = new QLabel("C++ fields");
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fileNameLabel);

    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
