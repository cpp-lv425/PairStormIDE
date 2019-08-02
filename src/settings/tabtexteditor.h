/*
 * class show current <style> <font name> <font size> in it fields
 * available preview for fonts
 * pressing OK/Apply button user can change one of these parameters.
 * in this case:
 *  - emits signal with argument std::map<QString, QString> arg;
 *    arg can have fields: "editorFontSize" "editorFont" "editorStyle"
 *    with new parametr
 *  - change current FontSize in QSettings
 *  - change current FontSize in this
*/
#ifndef TABTEXTEDITOR_H
#define TABTEXTEDITOR_H

#include <QWidget>
#include <QtWidgets>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class TabTextEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TabTextEditor(QWidget *parent = nullptr);
private:
    void setupFontTree();               //  show all fonts in widget

    // variables to handle Style
    QStringList editorStylesList         {"WHITE", "BLUE", "DARK"};
    QString styleDefault                = "WHITE";
    QComboBox *pcomboEditorStyle;

    // variables to handle Font size
    QStringList editorFontSizeList;     // range 6 .. 24
    QString editorFontSizeDefault       = "12";
    QComboBox *pcomboFontSize;

    // variables to handle Font
    QTreeWidget *fontTree;
    QString fontDefault     = "Consolas";
    size_t fontNumber       = 1;

    QTextEdit *textEdit;
public:                                          // map keep changed values in ComboBoxes
    std::map<QString, QString> newValuesToChange;// has fields: "editorFontSize" "editorFont" "editorStyle"
    QString fontCurrent;
    QString editorFontSizeCurrernt;
    QString styleCurrernt;
public slots:
    void showFont(QTreeWidgetItem *item);       // whow current font in preview window

    void onChangeStyle(const QString &);
    void onChangeFontSize(const QString &);

};

#endif // TABTEXTEDITOR_H
