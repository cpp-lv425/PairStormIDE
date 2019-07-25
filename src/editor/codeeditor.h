#ifndef CODEEDITOR_H
#define CODEEDITOR_H


#define TAB_SPACE 4

#include <QPlainTextEdit>
#include <QObject>
#include<QEvent>
#include<QKeyEvent>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;


class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void keyPressEvent(QKeyEvent *e) override;
protected:

    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);


private:
    QWidget *lineNumberArea;
};

#endif // CODEEDITOR_H
