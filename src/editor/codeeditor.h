#ifndef CODEEDITOR_H
#define CODEEDITOR_H


const int CHANGE_SAVE_TIME = 1000;
const int TAB_SPACE = 4;

#include<QPlainTextEdit>
#include<QObject>
#include"ideconfiguration.h"
#include"changemanager.h"
#include<utility>
#include<QAbstractScrollArea>
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
    bool isinsidebracket();
    static QString tabs;
    QString& getFileName();
    void setFileName(const QString &flename);
    std::pair<const QString &, const QString &> getChangedFileInfo();

    void undo();
    void redo();

protected:
    void resizeEvent(QResizeEvent *event)override;

private slots:
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void runLexerAndHighlight();

public slots:
    void keyPressEvent(QKeyEvent *e) override;
    void autotab();
    void saveStateInTheHistory();

signals:
    void changesAppeared();


private:
   QWidget *lineNumberArea;
   ConfigParams configParam;
   int currentZoom = 100;
   QFont font;
   QVector<Token> tokens;
   Highlightercpp *hcpp;
   LexerCPP *lcpp;
   QString fileName;
   ChangeManager *changeManager;
   QTimer *timer;
   LexerCPP lexer;

};

#endif // CODEEDITOR_H
