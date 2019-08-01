#ifndef TABTEXTEDITOR_H
#define TABTEXTEDITOR_H

#include <QWidget>
#include <QtWidgets>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
//class QPrinter;
class QTextEdit;
//class QTreeWidget;
//class QTreeWidgetItem;
QT_END_NAMESPACE

class TabTextEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TabTextEditor(QWidget *parent = nullptr);
private:
    void setupFontTree();

    // variables to handle Style
    QStringList editorStylesList        {"white", "blue", "dark"};
    QString styleDefault;                       // read it from QSettings
    QString styleCurrent                = "white";
    QComboBox *pcomboEditorStyle;

    // variables to handle Font size
    QStringList editorFontSizeList;             // range 6 .. 24
    QString editorFontSizeCurrent       = "8";
    QString editorFontSizeDefault       = "8";  // take it from QSettings
    QComboBox *pcomboFontSize;

    // variables to handle Font
    QString font            = "Courier 10 Pitch";   //  by default
    QString fontCurrent     = "Courier 10 Pitch";
    int fontNumber          = 1;

    QTreeWidget *fontTree;
    QTextEdit *textEdit;
                                                 // map keep changed values in ComboBoxes
    std::map<QString, QString> newValuesToChange;// has fields: "editorFontSizeChenged" "editorFontChenged" "editorStyleChenged"
public slots:
    void showFont(QTreeWidgetItem *item);

    void onChangeStyle(const QString &);
    void onChangeFontSize(const QString&);

};

#endif // TABTEXTEDITOR_H
