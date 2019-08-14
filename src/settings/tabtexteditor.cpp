#include "tabtexteditor.h"

TabTextEditor::TabTextEditor(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch(1);

    // getting data from QSettings
    QSettings s;
    mFontSizeCurrent = (s.contains("editorFontSize") ? s.value("editorFontSize").toString() : mFontSizeDefault);
    mFontCurrent = (s.contains("editorFontName") ? s.value("editorFontName").toString() : mFontDefault);
    mFontSizeNew = mFontSizeCurrent;
    mFontNew = mFontCurrent;

    //      FONT SIZE
    QHBoxLayout *fontSizeLayoutHoriz = new QHBoxLayout;//();
    fontSizeLayoutHoriz->addStretch(1);
    // set a label
    QLabel *fontSizeLabel = new QLabel(tr("Font size"));
    mpComboFontSize = new QComboBox(this);
    for (int i = minRange; i <= maxRange; i += 2)
    {
        mFontSizeList << QString::number(i);
    }
    mpComboFontSize->addItems(mFontSizeList);
    fontSizeLayoutHoriz->addWidget(fontSizeLabel);
    fontSizeLayoutHoriz->addWidget(mpComboFontSize);

    QVBoxLayout *fontSizeLayoutVert = new QVBoxLayout;
    fontSizeLayoutVert->addStretch(1);
    fontSizeLayoutVert->addLayout(fontSizeLayoutHoriz);
    fontSizeLayoutVert->addSpacing(100);
    fontSizeLayoutVert->addStretch(100);

    mainLayout->addLayout(fontSizeLayoutVert);
    mainLayout->addSpacing(mColumnSpacing);

    // set combobox acording to settings
    mpComboFontSize->setCurrentIndex(0);
    for(int i = 0; i < mFontSizeList.size(); ++i)
    {
        auto &item = mFontSizeList[i];
        if (item == mFontSizeCurrent)
        {
            mpComboFontSize->setCurrentIndex(i);
            break;
        }
    }
    connect(mpComboFontSize, SIGNAL(currentIndexChanged(const QString&)),
                             SLOT(onChangeFontSize(const QString&)));

    //      FONT NAME
    // handling system fonts
    setupFontTree();
    mainLayout->addWidget(mpFontTree);
    mainLayout->addSpacing(mColumnSpacing);

    //  preview selected font
    mpTextEdit = new QTextEdit(this);
    mainLayout->addWidget(mpTextEdit);
    showFont(mpFontTree->topLevelItem(mFontNumber - 1));

    mainLayout->addStretch(100);
    setLayout(mainLayout);

    connect(mpFontTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(showFont(QTreeWidgetItem*)));
}

const QString &TabTextEditor::getFontSizeCurrernt() const
{
    return mFontSizeCurrent;
}

const QString &TabTextEditor::getFontSizeNew() const
{
    return  mFontSizeNew;
}

void TabTextEditor::setFontSizeCurrernt(const QString & newFontSize)
{
    mFontSizeCurrent = newFontSize;
}

const QString &TabTextEditor::getFontCurrernt() const
{
    return mFontCurrent;
}

const QString &TabTextEditor::getFontNew() const
{
    return  mFontNew;
}

void TabTextEditor::setFontCurrernt(const QString & newFont)
{
    mFontCurrent = newFont;
}

void TabTextEditor::setupFontTree()
{
    QFontDatabase database;
    mpFontTree = new QTreeWidget;
    mpFontTree->setColumnCount(1);
    mpFontTree->setHeaderLabels(QStringList() << tr("Font"));

    int decentFontNumber = 0;
    bool foundFont = false;
    foreach (QString family, database.families()) {
        QTreeWidgetItem *familyItem = new QTreeWidgetItem(mpFontTree);
        familyItem->setText(0, family);

        ++decentFontNumber;
        if (family == mFontCurrent)
        {
            mFontNumber = decentFontNumber;
            familyItem->setSelected(true);
            foundFont = true;
        }
    }
    if (!foundFont) // in system not found decent font. choose first in database
    {
        mpFontTree->setCurrentItem(
            mpFontTree->topLevelItem(0),    // item
            0,                            // column
            QItemSelectionModel::Select   // command
        );
        mFontNumber = 1;
    }
}

void TabTextEditor::showFont(QTreeWidgetItem *item)
{
    if (!item)
        return;

    QString family;
    if (item->parent())
        family = item->parent()->text(0);
    else
        family = item->text(0);

    mFontNew = family;

    QString oldText = mpTextEdit->toPlainText().trimmed();
    bool modified = mpTextEdit->document()->isModified();
    mpTextEdit->clear();
    QFont font(family, 25, QFont::Normal, false);

    mpTextEdit->document()->setDefaultFont(font);

    QTextCursor cursor = mpTextEdit->textCursor();
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignCenter);
    cursor.insertBlock(blockFormat);

    if (modified)
        cursor.insertText(QString(oldText));
    else
        cursor.insertText(QString("%1").arg(family));

    mpTextEdit->document()->setModified(modified);
    emit onChangeFont(mFontNew);
}

void TabTextEditor::onChangeFont(const QString & newItem)
{
    mFontNew = newItem;
    qDebug() << "onChangeFont" << newItem;
}

void TabTextEditor::onChangeFontSize(const QString & newItem)
{
    mFontSizeNew = newItem;
    qDebug() << "onChangeFontSize" << newItem;
}
