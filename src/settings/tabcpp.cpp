#include "tabcpp.h"

#include <QHBoxLayout>
#include <QSettings>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QInputDialog>

//#include <QAbstractItemView>

TabCpp::TabCpp(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;//();
    mainLayout->addStretch(mBasicStretch);

    // getting data from QSettings
    QSettings s;
    mStandartCurrernt = (s.contains("cppStandart") ? s.value("cppStandart").toString() : mStandartDefault);
    mStandartNew = mStandartCurrernt;
    mCompilersList = (s.contains("cppCompilersList") ? s.value("cppCompilersList").toStringList() : mCompilersListDefault);
    if (mCompilersList.size() && !mCompilersList.at(0).size())
    {
        mCompilersList.removeAt(0);
    }

    //      C++ STANDART
    QVBoxLayout *standartLayout = new QVBoxLayout;//();
    standartLayout->addStretch(mBasicStretch);
    QLabel *labelCstandart = new QLabel(tr("C++ standart"));
    mpComboStandart = new QComboBox(this);
    mpComboStandart->addItems(mStandartList);
    standartLayout->addWidget(labelCstandart);
    standartLayout->addWidget(mpComboStandart);
    standartLayout->addStretch(mStretchFactorMain);
    mainLayout->addLayout(standartLayout);
    mainLayout->addSpacing(mColumnSpasing);

    // set combobox acording to settings
    mpComboStandart->setCurrentIndex(0);
    for(int i = 0; i < mStandartList.size(); ++i)
    {
        auto &item = mStandartList[i];
        if (item == mStandartCurrernt)
        {
            mpComboStandart->setCurrentIndex(i);
            break;
        }
    }
    connect(mpComboStandart, SIGNAL(currentIndexChanged(const QString&)),
                            SLOT(onChangeStandart(const QString&)));

    //  path to compilers
    QVBoxLayout *compilersLayout = new QVBoxLayout;//();
    compilersLayout->addStretch(mBasicStretch);
    QLabel *labelCompiler = new QLabel(tr("Compilers:"));
    mpCompilersList = new QListWidget(this);
    mpCompilersList->addItems(mCompilersList);
    mpCompilersList->setSelectionMode(QAbstractItemView::SingleSelection);

    compilersLayout->addWidget(labelCompiler);
    compilersLayout->addWidget(mpCompilersList);

    mpAddCompiler = new QPushButton("Add ...", this);
    mpAddCompiler->setMaximumWidth(mCompilerWidth);
    mpAddCompiler->setMinimumWidth(mCompilerWidth);
    compilersLayout->addWidget(mpAddCompiler);
    connect(mpAddCompiler, &QPushButton::clicked, this, &TabCpp::onAddCompilerClicked);
    connect(mpCompilersList, &QListWidget::itemDoubleClicked, this, &TabCpp::onCompilerListDoubleClicked);



    mainLayout->addLayout(compilersLayout);
    mainLayout->addSpacing(mColumnSpasing);

    mainLayout->addStretch(mStretchFactorMain);
    setLayout(mainLayout);
}

const QString &TabCpp::getStandartCurrernt() const
{
    return mStandartCurrernt;
}

const QString &TabCpp::getStandartNew() const
{
    return mStandartNew;
}

void TabCpp::setStandartCurrernt(const QString & newItem)
{
    mStandartCurrernt = newItem;
}

void TabCpp::onChangeStandart(const QString & newItem)
{
    mStandartNew = newItem;
}

void TabCpp::onAddCompilerClicked()
{
    QString text = QInputDialog::getText(this, "Create new path", "Type path to compiler, please ...");
    if (text.size())
    {
        mpCompilersList->addItem(text);     //  add item to widget
        mCompilersList << text;             //  add item ti list
        QSettings s;
        s.setValue("cppCompilersList", QVariant::fromValue(mCompilersList));
        emit compilersListChanged(mCompilersList);
    }
}

void TabCpp::onCompilerListDoubleClicked(QListWidgetItem *item)
{
    QString initialText = item->text();
    QString text = QInputDialog::getText(this, "Modify path"
                                         ,"Modify path to compiler, please, or clear to remove path."
                                         , QLineEdit::Normal
                                         , item->text());

    if (!text.size())           //  user removed item from list
    {
        mpCompilersList->takeItem(mpCompilersList->row(item)); // remove item from widget
        for (int i = 0; i < mCompilersList.size(); ++i)        // remove item from list
        {
            if (mCompilersList.at(i) == initialText)
            {
                mCompilersList.removeAt(i);
                break;
            }
        }
    }
    else
    {
        item->setText(text);
    }

    if (initialText != text)        //  user changed item in list
    {
        for (int i = 0; i < mCompilersList.size(); ++i)        // change item in list
        {
            if (mCompilersList.at(i) == initialText)
            {
                mCompilersList[i] = text;
                break;
            }
        }

        QSettings s;
        s.setValue("cppCompilersList", QVariant::fromValue(mCompilersList));

        emit compilersListChanged(mCompilersList);
    }
}
