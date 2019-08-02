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

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
           | QDialogButtonBox::Apply | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::clicked, this, &MenuOptions::onBtnClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    show();
}

void MenuOptions::onBtnApplyClicked()
{
    //      APPLY ALL CHANGES IN TABS
    // tab text editor
    if (tabTextEditor->newValuesToChange.size())
    {
        emit valuesForEditor(tabTextEditor->newValuesToChange);

        std::map<QString, QString>::iterator it;
        QSettings s;
        // check if font size was changed
        it = tabTextEditor->newValuesToChange.find("editorFontSize");
        if (it != tabTextEditor->newValuesToChange.end())
        {
            tabTextEditor->editorFontSizeCurrernt = it->second;
            s.setValue("analizerFontSize", it->second);
        }
        // check if editor style was changed
        it = tabTextEditor->newValuesToChange.find("editorStyle");
        if (it != tabTextEditor->newValuesToChange.end())
        {
            tabTextEditor->styleCurrernt = it->second;
            s.setValue("analizerStyle", it->second);
        }
        // check if font name was changed
        it = tabTextEditor->newValuesToChange.find("editorFont");
        if (it != tabTextEditor->newValuesToChange.end())
        {
            tabTextEditor->fontCurrent = it->second;
            s.setValue("analizerFontName", it->second);
        }
        // clear all changes
        tabTextEditor->newValuesToChange.clear();
    }


}

void MenuOptions::onBtnClicked(QAbstractButton *button)
{
    if (button->text() == "&Cancel")
        close();
    if (button->text() == "Apply")
        onBtnApplyClicked();
    if (button->text() == "&OK")
    {
        onBtnApplyClicked();
        close();
    }
}

