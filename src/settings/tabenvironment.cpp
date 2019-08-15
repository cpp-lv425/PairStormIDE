#include "tabenvironment.h"

#include <QDesktopServices>
#include <QVBoxLayout>
#include <QSettings>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QDebug>
#include <QUrl>
#include <QDir>

TabEnvironment::TabEnvironment(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;//(this);
    mainLayout->addStretch(mBasicStretch);
    mainLayout->setSpacing(mSpacing);

    // getting data from QSettings
    QSettings s;
    mStyleCurrernt = (s.contains("style") ? s.value("style").toString() : mStyleDefault);
    mStyleNew = mStyleCurrernt;
    QString userName = (s.contains("userName") ? s.value("userName").toString() : "");

    //      STYLE
    // set a label
    QLabel *editorStyleLabel = new QLabel(tr("Application style"), this);
    mpComboEditorStyle = new QComboBox(this);
    mpComboEditorStyle->setMaximumWidth(mEditorStyleWidth);
    mpComboEditorStyle->setMinimumWidth(mEditorStyleWidth);
    mpComboEditorStyle->addItems(mEditorStylesList);
    mainLayout->addWidget(editorStyleLabel);
    mainLayout->addWidget(mpComboEditorStyle);
    mainLayout->addSpacing(mRowSpasing);

    // set combobox acording to settings
    mpComboEditorStyle->setCurrentIndex(0);
    for(int i = 0; i < mEditorStylesList.size(); ++i)
    {
        auto &item = mEditorStylesList[i];
        if (item == mStyleCurrernt)
        {
            mpComboEditorStyle->setCurrentIndex(i);
            break;
        }
    }
    connect(mpComboEditorStyle,  SIGNAL(currentIndexChanged(const QString&)),
                                 SLOT(onChangeStyle(const QString&)));

    //  label with configuration file path
    QLabel *label = new QLabel("Configuration file:", this);
    mainLayout->addWidget(label);

    mCurrentPath = QDir::currentPath();
    mCurrentPath += QDir::separator();
    mCurrentPath += "conf";
    mCurrentPath += QDir::separator();
    mCurrentPath += userName;
    mCurrentPath += ".json";
    QFile file(mCurrentPath);
    QLabel *pathLabel {file.exists() ? new QLabel(mCurrentPath, this) : new QLabel("  ... not found.", this)};
    mainLayout->addWidget(pathLabel);

    //  modify configuration file
    QPushButton *pBtn = new QPushButton("Modify ...", this);
    pBtn->setMinimumWidth(mModifyBtnWidth);
    pBtn->setMaximumWidth(mModifyBtnWidth);
    mainLayout->addWidget(pBtn);
    pBtn->setEnabled(file.exists());
    connect(pBtn, &QPushButton::clicked, this, &TabEnvironment::onClicked);

    setLayout(mainLayout);
}

void TabEnvironment::onClicked()
{
    QDesktopServices::openUrl(QUrl(mCurrentPath));
}

void TabEnvironment::onChangeStyle(const QString & newItem)
{
    mStyleNew = newItem;
}
const QString & TabEnvironment::getStyleCurrernt() const
{
    return mStyleCurrernt;
}

const QString & TabEnvironment::getStyleNew() const
{
    return mStyleNew;
}

void TabEnvironment::setStyleCurrernt(const QString &newStyle)
{
    mStyleCurrernt = newStyle;
}

