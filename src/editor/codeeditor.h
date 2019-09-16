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
#include <QStringList>
#include<QAbstractScrollArea>
#include<QSettings>
#include<QApplication>
#include<QPlainTextEdit>
#include<documentmanager.h>
#include<QObject>
#include<QMouseEvent>
#include<QLabel>
#include<commentwidget.h>
#include<QVector>
#include<QFont>
#include<QStatusBar>
#include <QTimer>
#include <QList>
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
    CodeEditor(QWidget *parent = nullptr, const QString &fileName = "");
    virtual ~CodeEditor();
    void specialAreasRepaintEvent(QPaintEvent *event);
    void repaintButtonsArea(const int bottom, const int top, const int blockNumber);
    int getLineNumberAreaWidth();
    QString& getFileName();
    void setFileName(const QString &flename);
    std::pair<const QString &, const QString &> getChangedFileInfo();
    void undo();
    void redo();
    void zoom(const int val);
    bool isChanged();
    void setBeginTextState();
    const QByteArray& getBeginTextState()const;
    void setTextState(const QByteArray &beginTextState);

    LastRemoveKey getLastRemomeKey() const;
    void setLastRemomeKey(const LastRemoveKey &value);

    ConfigParams getConfigParam();
    void setConfigParam(const ConfigParams &configParam);

    //DB methods
    void readAllCommentsFromDB(QVector<Comment> mStartComments);
    QVector<Comment> getAllCommentsToDB();

    CommentDb getCommentGetter() const;

    QVector<Comment> getStartComments() const;

private:
    void rewriteButtonsLines(QVector<AddCommentButton*> &commentV, const int diff, const int startLine);
    void setAnotherButtonLine(AddCommentButton *comment, const int diff);
    bool isInRangeIncludBoth(const int val, const int leftMargin, const int rightMargin);
    bool isInRangeIncludLast(const int val, const int leftMargin, const int rightMargin);

    void handleLinesAddition(int, int, int);
    void handleLinesDelition(int, int);
    void addToIdentifiersList(QStringList&, int);
    void getNamesOfIdentifiers();

    void addButton(const int line, const QString &Comment, const QString &userName);
    void removeButtonByIndex(QVector<AddCommentButton*> &commentV, const int index);
    void removeButtomByValue(QVector<AddCommentButton*> &commentV, AddCommentButton* commentButton);
    void removeButtons(QVector<AddCommentButton*> &commentV, const int cursorLine,
                       const int startLine, const int endLine, const int diff);

    bool commentButtonExists(int line);
    AddCommentButton* getCommentButtonByIndex(const int line);
    void setNewAddedButtonSettings(AddCommentButton *commentButton);
    CodeEditor* getOpenedDocument(const QString &fileName);
    void runHighlighterWithDefinition(CodeEditor *sourceDocument);

protected:
    void resizeEvent(QResizeEvent *event)override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect &rect, const int dy);
    void handleLineChange(int);
    void highlightText();
    void deleteComment();

public slots:
    void keyPressEvent(QKeyEvent *e) override;
    void saveStateInTheHistory();
    void handleLinesSwap(int, int);
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
    void linesWasSwapped(int, int);
    void changesAppeared();
    void sendLexem(QString);
    void runHighlighter();
    void closeDocEventOccured(CodeEditor*);
    void textChangedInLine(int);
    void textChangedInLines(int, int);
    void linesCountUpdated();
    void openDocument(const QString &);

private:
    QWidget *mLineNumberArea;
    ConfigParams mConfigParam;
    QFont mFont;
    LexerCPP mLcpp;
    QString mFileName;
    ChangeManager mChangeManager;
    QTimer mTimer;
    LexerCPP mLexer;
    AddCommentButton *mAddCommentButton;
    CommentWidget mCommentWidget;
    QLabel mCurrentCommentLable;
    QVector<Comment> mStartComments;
    QCompleter *mCompleter;
    QStringList completerKeywords;
    CommentDb commentGetter;
    QSettings settings;


    QString mStyle;
    int mLinesCountPrev;
    int mLinesCountCurrent;

    unsigned int mLinesCount;
    unsigned int mCodeSize;
    QString mCode;

    unsigned int mHighlightingStart;

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
    QList<QVector<Token>> mTokensList;
    QList<QStringList> mIdentifiersList;
    QStringList mIdentifiersNameList;
    friend class Event;

    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // CODEEDITOR_H
