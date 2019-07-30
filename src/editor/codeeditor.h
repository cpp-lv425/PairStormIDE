#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include<QAbstractScrollArea>
#include <QPlainTextEdit>
#include <QObject>
#include "highlightercpp.h"
#include "lexercpp.h"
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
    QString& getFileName();
    void setFileName(const QString &flename);

protected:
    void resizeEvent(QResizeEvent *event)override;

private slots:
   void updateLineNumberAreaWidth();
   void highlightCurrentLine();
   void updateLineNumberArea(const QRect &rect, int dy);
   void runLexer();

public slots:
   void keyPressEvent(QKeyEvent *e) override;

private:
   QWidget *lineNumberArea;
   ConfigParams configParam;
   int currentZoom = 100;
   QFont font;
   QVector<Token> tokens;
   Highlightercpp *hcpp;
   LexerCPP *lcpp;
};



#endif // CODEEDITOR_H
