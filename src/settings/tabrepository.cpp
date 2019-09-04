#include "tabrepository.h"
#include <QHBoxLayout>
#include <QLabel>

TabRepository::TabRepository(QWidget *parent) : QWidget(parent)
{
    QLabel *fileNameLabel = new QLabel("Repository fields");
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fileNameLabel);

    mainLayout->addStretch(mBasicStretch);
    setLayout(mainLayout);
}
