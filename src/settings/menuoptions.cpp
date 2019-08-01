#include "menuoptions.h"
#include <QDebug>

#include <QVBoxLayout>
MenuOptions::MenuOptions(QWidget *pParent) : QDialog (pParent)
{
    setModal(true);
    int width = 850;
    int height = 280;
    setMinimumWidth(width);
    setMaximumWidth(width);
    setMinimumHeight(height);
    setMaximumHeight(height);
    setWindowTitle("Settings");

    tabWidget = new QTabWidget;

    tabEnvironment = new TabEnvironment(this);
    tabTextEditor = new TabTextEditor(this);
    tabCpp = new TabCpp(this);
    tabRepository = new TabRepository(this);
    tabConnection = new TabConnection(this);


    tabWidget->addTab(tabEnvironment, "Environment");
    tabWidget->addTab(tabTextEditor, "Text Editor");
    tabWidget->addTab(tabCpp, "C++");
    tabWidget->addTab(tabRepository, "Repository");
    tabWidget->addTab(tabConnection, "Connection");

    //buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
    //       | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);

    //connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    	//connect(buttonBox, &QDialogButtonBox::, this, &QDialog::accept);
    //connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    //mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    show();
}

