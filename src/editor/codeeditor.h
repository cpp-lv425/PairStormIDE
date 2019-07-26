#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include<QAbstractScrollArea>
#include <QPlainTextEdit>
#include <QObject>
#include "ideconfiguration.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;


class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    bool insidebracket();
    static QString tabs;
protected:
    void resizeEvent(QResizeEvent *event)override;

private slots:
   void updateLineNumberAreaWidth();
   void highlightCurrentLine();
   void updateLineNumberArea(const QRect &rect, int dy);

public slots:
   void keyPressEvent(QKeyEvent *e) override;
   void autotab();

private:
   QWidget *lineNumberArea;
   ConfigParams configParam;
   int currentZoom = 100;
   QFont font;

};


#endif // CODEEDITOR_H
