#include "tabtexteditor.h"

TabTextEditor::TabTextEditor(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch(1);

    QSettings s;
    QString styleFromSettings = s.value("analizerStyle").toString();
    QString sizeFromSettings = s.value("analizerFontSize").toString();
    QString fontFromSettings = s.value("analizerFontName").toString();
    qDebug() << "FromSettings: style" << styleFromSettings << "size"
             << sizeFromSettings << "font" << fontFromSettings;

    QLabel *editorStyle = new QLabel(tr(" Editor style"));
    pcomboEditorStyle = new QComboBox(this);
    pcomboEditorStyle->addItems(editorStylesList);
    mainLayout->addWidget(editorStyle);
    mainLayout->addWidget(pcomboEditorStyle);
    pcomboEditorStyle->setCurrentIndex(0);
    connect(pcomboEditorStyle,  SIGNAL(currentIndexChanged(const QString&)),
                                SLOT(onChangeStyle(const QString&)));

    QLabel *fontSoze = new QLabel(tr(" Font size"));
    pcomboFontSize = new QComboBox(this);
    for (int i = 6; i < 25; i += 2)
        editorFontSizeList << QString::number(i);
    pcomboFontSize->addItems(editorFontSizeList);
    mainLayout->addWidget(fontSoze);
    mainLayout->addWidget(pcomboFontSize);
    pcomboFontSize->setCurrentIndex(1);
    connect(pcomboFontSize, SIGNAL(currentIndexChanged(const QString&)),
                            SLOT(onChangeFontSize(const QString&)));

    setupFontTree();
    mainLayout->addWidget(fontTree);
    textEdit = new QTextEdit;
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

    size_t i = 0;
    foreach (QString family, database.families()) {
        const QStringList styles = database.styles(family);
        if (styles.isEmpty())
            continue;

        QTreeWidgetItem *familyItem = new QTreeWidgetItem(fontTree);
        familyItem->setText(0, family);

        ++i;
        if (family == fontCurrent)
        {
            fontNumber = i;
            familyItem->setSelected(true);
        }
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
    qDebug() << "fontCurrent" << fontCurrent;
    newValuesToChange["editorFontChenged"] = family;// add this change to std::map<QString type, QString new value> of changes

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
    qDebug() << "onChangeStyle" << itemText;
    newValuesToChange["editorStyleChenged"] = itemText;// add this change to std::map<QString type, QString new value> of changes
        //      on Ok/Apply button:
        //      emmit signal to change style in editor
        //      change current style in conf.json ???
        //      change current style in QSettings
        //      change current style in this
}

void TabTextEditor::onChangeFontSize(const QString & itemText)
{
    qDebug() << "onChangeFontSize" << itemText;
    newValuesToChange["editorFontSizeChenged"] = itemText;// add this change to std::map<QString type, QString new value> of changes
    //      on Ok/Apply button:
    //      emmit signal to change FontSize in editor
    //      change current FontSize in conf.json ???
    //      change current FontSize in QSettings
    //      change current FontSize in this
}
