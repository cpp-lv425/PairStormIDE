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

bool isValidMethodInitialization(QTextCursor cursor)
{
    QString line = getTextByCursor(cursor);
    QRegularExpressionMatchIterator matchIter = QRegularExpression(validFucntionDefinition).
            globalMatch(line);
    return matchIter.hasNext();//rewrite in the future. now it's just valid every time
}

MethodDefinitionPattern getMethodDefinitionPattern(const QString &difinition)
{
    QRegularExpressionMatchIterator matchIter = QRegularExpression(validFucntionDefinition).
            globalMatch(difinition);
    QRegularExpressionMatch match = matchIter.next();// get match
    return MethodDefinitionPattern {match.captured(1), match.captured(2), match.captured(3)};
}

QString createFilePath(const QString &rootPath, const QString &file)
{
    return rootPath + '/' + file;
}

bool definitionExists(const QString &documentText, QTextCursor cursor)
{
    QString methodFullName = getMethodFullName(getTextByCursor(cursor));
}

QString getMethodFullName(QTextCursor cursor)
{
    auto declarationParts = getMethodDefinitionPattern(getTextByCursor(cursor));
    auto className = getClassNameForMethodDefinition(cursor);
    return className + "::" + declarationParts.fucntionName;
}
/*struct MethodDefinitionPattern
{
    QString functionDataType;
    QString fucntionName;
    QString functionParametrs;
};
*/
