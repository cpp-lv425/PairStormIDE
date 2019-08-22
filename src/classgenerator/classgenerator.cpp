#include "classgenerator.h"
#include "ui_classgenerator.h"

ClassGenerator::ClassGenerator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassGenerator)
{
    ui->setupUi(this);
}

ClassGenerator::~ClassGenerator()
{
    delete ui;
}
