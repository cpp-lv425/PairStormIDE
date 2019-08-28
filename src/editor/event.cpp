#include "event.h"

QString Event::sTabs="";
bool Event::sIsSleshPressed=false;
void Event::eventInsertSymbol(CodeEditor *codeEditor, QKeyEvent *e, QString s)
{
        plainTextPressEvent(codeEditor, e);
        codeEditor->insertPlainText(s);
        for (int i = 0; i < s.size(); i++)
        {
            codeEditor->moveCursor(QTextCursor::Left);
        }
}

bool Event::IsInsideBracket(CodeEditor *codeEditor)
{
    QTextCursor cursor = codeEditor->textCursor();
    if(cursor.position() != codeEditor->document()->toPlainText().size())
    {
        cursor.movePosition(QTextCursor::PreviousCharacter);
        int currentpos = cursor.position();
        QString prev = codeEditor->document()->toPlainText().at(currentpos);
        cursor.movePosition(QTextCursor::NextCharacter);
        currentpos = cursor.position();
        QString next = codeEditor->document()->toPlainText().at(currentpos);
        if(prev == "{" && next == "}")
        {
            return true;
        }
    }
    return false;
}

void Event::plainTextPressEvent(CodeEditor *codeEditor, QKeyEvent *e)
{
    codeEditor->QPlainTextEdit::keyPressEvent(e);
}

void Event::autotab(CodeEditor *code)
{
    QString text(code->document()->toRawText());
    int lbrackets = 0;
    int rbrackets = 0;
    QTextCursor crs = code->textCursor();
    for (int c = 0; c < crs.position(); c++)
    {
        if(text.at(c) == "{")
        {
            lbrackets++;
        }
        if(text.at(c) == "}")
        {
            rbrackets++;
        }
    }
    sTabs = "";
    int difference = lbrackets - rbrackets;
    for(int i = 0; i < difference; i++)
    {
      sTabs.append("\t");
    }
}


int Event::editorCurrentZoom(CodeEditor *codeEditor)
{
    return codeEditor->mCurrentZoom;
}

QList<QVector<Token>> Event::editorTokens(CodeEditor *codeEditor)
{
    return codeEditor->mTokensList;
}
