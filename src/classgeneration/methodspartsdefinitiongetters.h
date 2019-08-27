#ifndef METHODSPARTSDEFINITIONGETTERS_H
#define METHODSPARTSDEFINITIONGETTERS_H

#include<QString>
#include<QTextCursor>

struct MethodDefinitionPattern
{
    QString functionDataType;
    QString fucntionName;
    QString functionParametrs;
};

QString removeExtension(QString str, const int extensionCharacterCount);

QString createMethodDefinitionBones(const QString &dataType,  const QString &className,
                                    const QString &methodName,const QString &methodParams);
QString getTextByCursor(QTextCursor cursor);

QString getClassNameForMethodDefinition(QTextCursor cursor);

bool isValidMethodInitialization(QTextCursor cursor);

MethodDefinitionPattern getMethodDefinitionPattern(const QString &difinition);

QString createFilePath(const QString &rootPath, const QString &file);

#endif // METHODSPARTSDEFINITIONGETTERS_H
