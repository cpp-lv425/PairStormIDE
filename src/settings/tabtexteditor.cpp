#include "tabtexteditor.h"

TabTextEditor::TabTextEditor(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch(1);

    // getting data from QSettings
    QSettings s;
    styleCurrernt = (s.contains("analizerStyle") ? s.value("analizerStyle").toString() : styleDefault);
    editorFontSizeCurrernt = (s.contains("analizerFontSize") ? s.value("analizerFontSize").toString() : editorFontSizeDefault);
    fontCurrent = (s.contains("analizerFontName") ? s.value("analizerFontName").toString() : fontDefault);

    //      STYLE
    // set a label
    QLabel *editorStyle = new QLabel(tr(" Editor style"));
    pcomboEditorStyle = new QComboBox(this);
    pcomboEditorStyle->addItems(editorStylesList);
    mainLayout->addWidget(editorStyle);
    mainLayout->addWidget(pcomboEditorStyle);

    // set combobox acording to settings
    pcomboEditorStyle->setCurrentIndex(0);
    for(int i = 0; i < editorStylesList.size(); ++i)
    {
        auto &item = editorStylesList[i];
        if (item == styleCurrernt)
            pcomboEditorStyle->setCurrentIndex(i);
    }
    connect(pcomboEditorStyle,  SIGNAL(currentIndexChanged(const QString&)),
                                SLOT(onChangeStyle(const QString&)));
    //      FONT SIZE
    // set a label
    QLabel *fontSize = new QLabel(tr(" Font size"));
    pcomboFontSize = new QComboBox(this);
    for (int i = 6; i < 25; i += 2)
        editorFontSizeList << QString::number(i);
    pcomboFontSize->addItems(editorFontSizeList);
    mainLayout->addWidget(fontSize);
    mainLayout->addWidget(pcomboFontSize);

    // set combobox acording to settings
    pcomboFontSize->setCurrentIndex(0);
    for(int i = 0; i < editorFontSizeList.size(); ++i)
    {
        auto &item = editorFontSizeList[i];
        if (item == editorFontSizeCurrernt)
        pcomboFontSize->setCurrentIndex(i);
    }
    connect(pcomboFontSize, SIGNAL(currentIndexChanged(const QString&)),
                            SLOT(onChangeFontSize(const QString&)));
    //      FONT NAME
    // handling system fonts
    setupFontTree();
    mainLayout->addWidget(fontTree);
    textEdit = new QTextEdit;                       //  preview font
    mainLayout->addWidget(textEdit);
    showFont(fontTree->topLevelItem(fontNumber - 1));

    setLayout(mainLayout);

    connect(fontTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(showFont(QTreeWidgetItem*)));
}

void TabTextEditor::setupFontTree()
{
    QFontDatabase database;
    fontTree = new QTreeWidget;
    fontTree->setColumnCount(1);
    fontTree->setHeaderLabels(QStringList() << tr("Font"));

    size_t decentFontNumber = 0;
    bool foundFont = false;
    foreach (QString family, database.families()) {
        const QStringList styles = database.styles(family);
        if (styles.isEmpty())
            continue;

        QTreeWidgetItem *familyItem = new QTreeWidgetItem(fontTree);
        familyItem->setText(0, family);

        ++decentFontNumber;
        if (family == fontCurrent)
        {
            fontNumber = decentFontNumber;
            familyItem->setSelected(true);
            foundFont = true;
        }
    }
    if (!foundFont) // in system not founded decent font. choose first in database
    {
        fontTree->setCurrentItem(
            fontTree->topLevelItem(0),    // item
            0,                            // column
            QItemSelectionModel::Select   // command
        );
        fontNumber = 1;
    }
}

void TabTextEditor::showFont(QTreeWidgetItem *item)
{
    if (!item)
        return;

    QString family;
    QString style;
    if (item->parent())
        family = item->parent()->text(0);
    else
        family = item->text(0);

    fontCurrent = family;
    //qDebug() << "fontCurrent" << fontCurrent;
    newValuesToChange["editorFont"] = family;// add this change to std::map<QString type, QString new value> of changes

    QString oldText = textEdit->toPlainText().trimmed();
    bool modified = textEdit->document()->isModified();
    textEdit->clear();
    QFont font(family, 25, QFont::Normal, false);

    textEdit->document()->setDefaultFont(font);

    QTextCursor cursor = textEdit->textCursor();
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignCenter);
    cursor.insertBlock(blockFormat);

    if (modified)
        cursor.insertText(QString(oldText));
    else
        cursor.insertText(QString("%1").arg(family));

    textEdit->document()->setModified(modified);
}

void TabTextEditor::onChangeStyle(const QString & itemText)
{
    newValuesToChange["editorStyle"] = itemText;// add this change to std::map<QString type, QString new value> of changes
        //      on Ok/Apply button:
        //      emmit signal to change style in editor
        //      change current style in QSettings
        //      change current style in this
}

void TabTextEditor::onChangeFontSize(const QString & itemText)
{
    newValuesToChange["editorFontSize"] = itemText;// add this change to std::map<QString type, QString new value> of changes
    //      on Ok/Apply button:
    //      emmit signal to change FontSize in editor
    //      change current FontSize in QSettings
    //      change current FontSize in this
}
