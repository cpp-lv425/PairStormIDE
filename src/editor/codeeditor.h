#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#define CHANGE_SAVE_TIME 1000
#define TAB_SPACE 4

#include<QPlainTextEdit>
#include<QObject>
#include"ideconfiguration.h"
#include"changemanager.h"
#include<utility>
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
    void setFileName(const QString &fileame);
    std::pair<const QString &, const QString &> getChangedFileInfo();


protected:
    void resizeEvent(QResizeEvent *event)override;

private slots:
   void updateLineNumberAreaWidth();
   void highlightCurrentLine();
   void updateLineNumberArea(const QRect &rect, int dy);
   void runLexer();

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
   ChangeManager *changeManager;
   QTimer *timer;
   LexerCPP lexer;
   QVector<Token> tokens;
};

#endif // CODEEDITOR_H
