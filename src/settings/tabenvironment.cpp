#include "tabenvironment.h"

#include <QtWidgets>

TabEnvironment::TabEnvironment(QWidget *parent)
    : QWidget(parent)
{
    QLabel *fileNameLabel = new QLabel("Environment fields");
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fileNameLabel);

    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
