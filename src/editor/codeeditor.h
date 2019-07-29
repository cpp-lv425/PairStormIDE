#ifndef CODEEDITOR_H
#define CODEEDITOR_H



#include<QPlainTextEdit>
#include<QObject>
#include"ideconfiguration.h"
#include"changemanager.h"
#include<utility>

#define CHANGE_SAVE_TIME 3000

#include<QAbstractScrollArea>
#include <QPlainTextEdit>
#include <QObject>
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
    bool insidebracket();
    static QString tabs;

    QString& getFileName();

    void setFileName(const QString &flename);

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

   void autotab();


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
   LexerCPP lexer;
   QVector<Token> tokens;

};



#endif // CODEEDITOR_H
