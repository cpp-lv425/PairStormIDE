#ifndef CODEEDITOR_H
#define CODEEDITOR_H


#include<QPlainTextEdit>
#include<QObject>
#include"ideconfiguration.h"
#include"changesmanager.h"

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

public slots:
   void keyPressEvent(QKeyEvent *e) override;
   void saveStateInTheHistory();

private:
   QWidget *lineNumberArea;
   ConfigParams configParam;
   int currentZoom = 100;
   QFont font;
   QString fileName;
   ChangesManager changesManager;
   QTimer *timer;
};



#endif // CODEEDITOR_H
