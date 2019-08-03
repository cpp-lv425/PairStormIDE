#include "event.h"

QString Event::tabs="";

void Event::eventInsertSymbol(CodeEditor *codeEditor, QKeyEvent *e, QString s)
{
        plainTextPressEvent(codeEditor,e);
        codeEditor->insertPlainText(s);
        codeEditor->moveCursor(QTextCursor::Left);
        codeEditor->verticalScrollBar();
}

bool Event::isinsidebracket(CodeEditor * codeEditor)
{
    QTextCursor cursor = codeEditor->textCursor();
    if(cursor.position()!=codeEditor->document()->toPlainText().size())
    {
        cursor.movePosition(QTextCursor::PreviousCharacter);
        int currentpos = cursor.position();
        QString prev =codeEditor->document()->toPlainText().at(currentpos);
        cursor.movePosition(QTextCursor::NextCharacter);
        currentpos = cursor.position();
        QString next = codeEditor->document()->toPlainText().at(currentpos);
        if(prev == "{" && next == "}")
            return true;
    }
    return false;
}

void Event::plainTextPressEvent(CodeEditor *codeEditor, QKeyEvent *e)
{
    codeEditor->QPlainTextEdit::keyPressEvent(e);
}

/*void Event::autotab()
{
    QString text(this->document()->toRawText());
    int lbrackets = 0;
    int rbrackets = 0;
    QTextCursor crs = this->textCursor();
    for (int c = 0; c < crs.position(); c++)
    {
        if(text.at(c) == "{")
            lbrackets++;
        if(text.at(c) == "}")
            rbrackets++;
    }
    tabs = "";
    int difference = lbrackets - rbrackets;

    for(int i = 0; i < difference; i++)
    {
      tabs.append("\t");
    }
}
*/
