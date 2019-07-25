#ifndef CODEEDITOR_H
#define CODEEDITOR_H
#define TAB_SPACE 4
#define MAX_SUPPORTED_DIGITS 5

#include <QPlainTextEdit>
#include <QObject>
#include<QEvent>
#include<QKeyEvent>
#include"ideconfiguration.h"
//#include"somefunction.h"

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
protected:
    void resizeEvent(QResizeEvent *event);
  // void CodeEditor::wheelEvent(QWheelEvent *event);

private slots:
   void updateLineNumberAreaWidth(int newBlockCount);
   void highlightCurrentLine();
   void updateLineNumberArea();
public slots:
   void keyPressEvent(QKeyEvent *e) override;
private:
   QWidget *lineNumberArea;
   ConfigParams configParam;
};



#endif // CODEEDITOR_H
