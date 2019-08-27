#include "methodspartsdefinitiongetters.h"
#include "definitionindexes.h"
#include "classgenerationliterals.h"
#include<QRegularExpression>
#include<QDebug>

QString getTextByCursor(QTextCursor cursor)
{
    cursor.select(QTextCursor::LineUnderCursor);
    return cursor.selectedText();
}

//QPair<QString, QString> getMethodNameFromFullDefinition(QString definition)
//{
//   QStringList listSplitedByBracket = definition.split('(', QString::SkipEmptyParts);
//   QStringList listSplitedBySpace = listSplitedByBracket[leftPartOfDefinitionIndex].
//           split(' ', QString::SkipEmptyParts);

//   return qMakePair(listSplitedBySpace[indexOfMethodTypeInLine].simplified(),
//                    listSplitedBySpace[indexOfMethodNameInLine]);
//}

//QString getMethodParametrsFromFullDefinition(QString definition)
//{
//    QRegularExpressionMatchIterator matchIter = QRegularExpression(textInsideBracketsRegex).
//            globalMatch(definition);
//    return matchIter.hasNext() ? matchIter.next().capturedTexts()[0] : QString();
//}

QString getClassNameForMethodDefinition(QTextCursor cursor)
{
    while (cursor.blockNumber())
    {
        QString textInCursorLine = getTextByCursor(cursor);
        if (textInCursorLine.contains("class")
           && !textInCursorLine.contains("friend"))
        {
            return getTextByCursor(cursor).split(' ')[indexOfClassNameInLine];//rewrite to regex
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

MethodDefinitionPattern getMethodDefinitionPatter(QString difinition)
{
    MethodDefinitionPattern rMethodDefinitionPattern;
    QRegularExpressionMatchIterator matchIter = QRegularExpression(validFucntionDefinition).
            globalMatch(difinition);
    if(!matchIter.hasNext())
    {
        return {QString(), QString(), QString()};
    }
        QRegularExpressionMatch match = matchIter.next();// get match
    rMethodDefinitionPattern.functionDataType = match.captured(1);
    qDebug()<<"type = "<<match.captured(1);

    rMethodDefinitionPattern.fucntionName = match.captured(2);
    qDebug()<<"name = "<<match.captured(2);

    rMethodDefinitionPattern.functionParametrs = match.captured(3);
     qDebug()<<"parametrs = "<<match.captured(3);
    return rMethodDefinitionPattern;
}
