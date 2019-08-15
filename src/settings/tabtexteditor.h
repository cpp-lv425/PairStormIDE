/*
 * class provides services:
 *  changing editor' font
 *  changing editor' font size
 *  show selected font
*/
#ifndef TABTEXTEDITOR_H
#define TABTEXTEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QComboBox;
class QTextEdit;
class QTreeWidgetItem;
QT_END_NAMESPACE

class TabTextEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TabTextEditor(QWidget *parent = nullptr);

    const QString & getFontSizeCurrernt() const;
    const QString & getFontSizeNew() const;
    void setFontSizeCurrernt(const QString &);

    const QString & getFontCurrernt() const;
    const QString & getFontNew() const;
    void setFontCurrernt(const QString &);

private:
    void setupFontTree();

    // variables to handle Font size
    QStringList mFontSizeList;     // range [minRange .. maxRange]
    int minRange                    = 6;
    int maxRange                    = 24;
    QString mFontSizeDefault        = "12";
    QString mFontSizeCurrent;
    QString mFontSizeNew;
    QComboBox *mpComboFontSize;

    // variables to handle Font
    QTreeWidget *mpFontTree;
    QString mFontDefault            = "Consolas";
    QString mFontCurrent;
    QString mFontNew;
    int mFontNumber                 = 1;

    QTextEdit *mpTextEdit;

    // design parameters
    int mFontSizePreview            = 25;
    int mColumnSpacing              = 10;
    int mBasicStretch               = 1;

public slots:
    void showFont(QTreeWidgetItem *item);       // show current font in preview window

    void onChangeFont(const QString &);
    void onChangeFontSize(const QString &);
};

#endif // TABTEXTEDITOR_H
