#include "tabrepository.h"
#include <QtWidgets>

TabRepository::TabRepository(QWidget *parent) : QWidget(parent)
{
    QLabel *fileNameLabel = new QLabel("Repository fields");
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fileNameLabel);

    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
