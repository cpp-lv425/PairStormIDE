#ifndef CODEEDITOR_H
#define CODEEDITOR_H

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

protected:
    void resizeEvent(QResizeEvent *event)override;

private slots:
   void updateLineNumberAreaWidth();
   void highlightCurrentLine();
   void updateLineNumberArea(const QRect &rect, int dy);

public slots:
   void keyPressEvent(QKeyEvent *e) override;
private:
   QWidget *lineNumberArea;
   ConfigParams configParam;
<<<<<<< HEAD
   int currentZoom = 100;
=======
/*public slots:
    void keyPressEvent(QKeyEvent *e) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event)override;
  //  void wheelEvent(QWheelEvent *event)override;
private slots:
    void highlightCurrentLine();
    void updateLineNumberArea(QRect rect, int dy);

private:
    QWidget *lineNumberArea;
    ConfigParams configParam;
signals:
    void returnPressed();
    void backspacePressed();
    void ctrlPlusVPressed();
    void wheelScroled();
    void ctrlPlusVPressed();*/
>>>>>>> b136e5bd2c07ae69e4ae75fe38213344e370dbdc
};



#endif // CODEEDITOR_H
