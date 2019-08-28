#ifndef CODEEDITOR_H
#define CODEEDITOR_H

const int CHANGE_SAVE_TIME = 1000;
const int TAB_SPACE = 4;
const int TOP_UNUSED_PIXELS_HEIGHT = 4;

#include"ideconfiguration.h"
#include"changemanager.h"
#include"addcommentbutton.h"
#include"addcommenttextedit.h"
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
#include<commentwidget.h>
#include<QVector>
#include<QFont>
#include<QStatusBar>
#include"sqliteaccess.h"
#include<QStringList>
#include<QCompleter>
#include"autocodecompleter.h"



class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class LineNumberArea;


enum LastRemoveKey
{
    BACK,
    DEL
};

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr);
    void specialAreasRepaintEvent(QPaintEvent *event);
    void repaintButtonsArea(int bottom, int top, int blockNumber);
    int getLineNumberAreaWidth();
    bool isinsidebracket();
    QString& getFileName();
    void setFileName(const QString &flename);
    std::pair<const QString &, const QString &> getChangedFileInfo();
    void undo();
    void redo();
    void zoom(int val);
    bool isChanged();
    void setBeginTextState();
    const QByteArray& getBeginTextState()const;
    void setTextState(const QByteArray &beginTextState);

    LastRemoveKey getLastRemomeKey() const;
    void setLastRemomeKey(const LastRemoveKey &value);

    ConfigParams getConfigParam();
    void setConfigParam(const ConfigParams &configParam);
    void highlighText();

    //DB methods
    void readAllCommentsFromDB(QVector<Comment> mStartComments);
    QVector<Comment> getAllCommentsToDB();

private:
    void rewriteButtonsLines( QVector<AddCommentButton*> &commentV, int diff, int startLine);
    void setAnotherButtonLine(AddCommentButton *comment, int diff);
    bool isInRangeIncludBoth(int val, int leftMargin, int rightMargin);
    bool isInRangeIncludLast(int val, int leftMargin, int rightMargin);

    void addButton(const int line, const QString &Comment);
    void removeButtonByIndex(QVector<AddCommentButton*> &commentV, int index);
    void removeButtomByValue(QVector<AddCommentButton*> &commentV, AddCommentButton* commentButton);
    void removeButtons(QVector<AddCommentButton*> &commentV, int cursorLine, int startLine, int endLine, int diff);

    bool commentButtonExists(int line);
    AddCommentButton* getCommentButtonByIndex(const int line);
    void setNewAddedButtonSettings(AddCommentButton *commentButton);

protected:
    void resizeEvent(QResizeEvent *event)override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect &rect, int dy);
    void runLexer();
    void deleteComment();

public slots:
    void keyPressEvent(QKeyEvent *e) override;
    void saveStateInTheHistory();
    void setZoom(int zoomVal);
    void textChangedInTheOneLine();
    void showCommentTextEdit(int);
    void emptyCommentWasAdded();
    void notEmptyCommentWasAdded();
    void changeCommentButtonsState();
    void setTextColors();
    void setFontSize(const QString &fontSize);
    void setFontStyle(const QString &fontStyle);
    void setIdeType(const QString &ideType);
    void writeDefinitionToSource();

signals:
    void changesAppeared();
    void sendLexem(QString);
    void closeDocEventOccured(CodeEditor*);
    void textChangedInLine(int);
    void textChangedInLines(int, int);
    void linesCountUpdated();

private:
    QWidget *mLineNumberArea;
    ConfigParams mConfigParam;
    QFont mFont;
    LexerCPP *mLcpp;
    QString mFileName;
    ChangeManager *mChangeManager;
    QTimer *mTimer;
    LexerCPP mLexer;
    AddCommentButton *mAddCommentButton;
    CommentWidget *mCommentWidget;
    QLabel *mCurrentCommentLable;
    QVector<Comment> mStartComments;
    QCompleter *mCompleter;
    QStringList completerKeywords;

    int mLinesCountPrev;
    int mLinesCountCurrent;

    QByteArray mBeginTextState;
    QVector<AddCommentButton*> mCommentsVector;

    QTextCharFormat fmtLiteral;
    QTextCharFormat fmtComment;
    QTextCharFormat fmtKeyword;
    QTextCharFormat fmtRegular;
    QTextCharFormat fmtUndefined;


    LastRemoveKey lastRemomeKey;

protected:
    int mCurrentZoom;
    QVector<Token> mTokens;
    friend class Event;

    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // CODEEDITOR_H
