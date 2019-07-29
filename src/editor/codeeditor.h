#ifndef CODEEDITOR_H
#define CODEEDITOR_H


#include<QPlainTextEdit>
#include<QObject>
#include"ideconfiguration.h"
#include"changemanager.h"
#include<utility>

#define CHANGE_SAVE_TIME 3000

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
    void setFileName(const QString &fileame);
    std::pair<const QString &, const QString &> getChangedFileInfo();


protected:
    void resizeEvent(QResizeEvent *event)override;

private slots:
   void updateLineNumberAreaWidth();
   void highlightCurrentLine();
   void updateLineNumberArea(const QRect &rect, int dy);

public
slots:
   void keyPressEvent(QKeyEvent *e) override;
   void saveStateInTheHistory();
signals:
   void changesAppeared();

private:
   QWidget *lineNumberArea;
   ConfigParams configParam;
   int currentZoom = 100;
   QFont font;
   QString fileName;
   ChangeManager changeManager;
   QTimer *timer;
};



#endif // CODEEDITOR_H
