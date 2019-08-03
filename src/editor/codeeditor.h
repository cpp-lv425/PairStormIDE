#ifndef CODEEDITOR_H
#define CODEEDITOR_H

const int CHANGE_SAVE_TIME = 1000;
const int TAB_SPACE = 4;
const int TOP_UNUSED_PIXELS_HEIGHT = 4;

#include"ideconfiguration.h"
#include"changemanager.h"
#include"addcommentbutton.h"
#include"ideconfiguration.h"
#include"lexercpp.h"
#include<utility>
#include<QAbstractScrollArea>
#include<QSettings>
#include<QApplication>
#include<QPlainTextEdit>
#include<QObject>
#include<QMouseEvent>
#include<QLabel>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int getLineNumberAreaWidth();
    bool isinsidebracket();
    static QString tabs;
    QString& getFileName();
    void setFileName(const QString &flename);
    std::pair<const QString &, const QString &> getChangedFileInfo();
    void undo();
    void redo();
    void zoom(int val);
    bool isChanged();
    void setBeginTextState();

protected:
    void resizeEvent(QResizeEvent *event)override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect &rect, int dy);
    void runLexer();
    void highlighText();

public slots:
    void keyPressEvent(QKeyEvent *e) override;
    void autotab();
    void saveStateInTheHistory();

signals:
    void changesAppeared();
    void sendLexem(QString);
    void closeDocEventOccured(CodeEditor*);

private:
    QWidget *mLineNumberArea;
    ConfigParams mConfigParam;
    int mCurrentZoom;
    QFont mFont;
    QVector<Token> mTokens;
    LexerCPP *mLcpp;
    QString mFileName;
    ChangeManager *mChangeManager;
    QTimer *mTimer;
    LexerCPP mLexer;
    AddCommentButton *mAddCommentButton;
    QLabel *mCurrentCommentLable;
    int mLinesCount;
    QString mBeginTextState;

    QTextCharFormat fmtLiteral;
    QTextCharFormat fmtComment;
    QTextCharFormat fmtKeyword;
    QTextCharFormat fmtRegular;
};

#endif // CODEEDITOR_H
