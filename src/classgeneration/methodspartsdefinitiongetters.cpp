#include "methodspartsdefinitiongetters.h"
#include "definitionindexes.h"
#include "classgenerationliterals.h"
#include<QRegularExpression>

QString getTextByCursor(QTextCursor cursor)
{
    cursor.select(QTextCursor::LineUnderCursor);
    return cursor.selectedText();
}

QPair<QString, QString> getMethodNameFromFullDefinition(QString definition)
{
   QStringList listSplitedByBracket = definition.split('(', QString::SkipEmptyParts);
   QStringList listSplitedBySpace = listSplitedByBracket[leftPartOfDefinitionIndex].
           split(' ', QString::SkipEmptyParts);

   return qMakePair(listSplitedBySpace[indexOfMethodTypeInLine].simplified(),
                    listSplitedBySpace[indexOfMethodNameInLine]);
}

QString getMethodParametrsFromFullDefinition(QString definition)
{
    QRegularExpressionMatchIterator matchIter = QRegularExpression(textInsideBracketsRegex).
            globalMatch(definition);
    return matchIter.hasNext() ? matchIter.next().capturedTexts()[0] : QString();
}

QString getClassNameForMethodDefinition(QTextCursor cursor)
{
    //before it we should be sure that method bone exists(type name(parametrs). in other way, don't show add definition menu)
    while (cursor.columnNumber())
    {
        QString textInCursorLine = getTextByCursor(cursor);
        if (textInCursorLine.contains("class")
           && !textInCursorLine.contains("friend"))
        {
            return getTextByCursor(cursor).split(' ')[indexOfClassNameInLine];
        }
        cursor.movePosition(QTextCursor::Up);
    }
    return QString();
}

bool definitionExists(QTextCursor cursor)
{

    QString line = getTextByCursor(cursor);
    QRegularExpressionMatchIterator matchIter = QRegularExpression(validFucntionDefinition).
            globalMatch(line);
    return matchIter.hasNext();//rewrite in the future. now it's just valid every time
}
