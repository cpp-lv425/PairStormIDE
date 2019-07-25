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

public slots:
    void keyPressEvent(QKeyEvent *e) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    //void wheelEvent(QWheelEvent *event)override;
private slots:
    void highlightCurrentLine();
    void updateLineNumberArea();

private:
    QWidget *lineNumberArea;
    ConfigParams configParam;
signals:
    void returnPressed();
    void backspacePressed();
    void ctrlPlusVPressed();
};



#endif // CODEEDITOR_H
