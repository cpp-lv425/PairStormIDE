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

//QPair<QString, QString> getMethodNameFromFullDefinition(QString definition);
//QString getMethodParametrsFromFullDefinition(QString definition);

QString getClassNameForMethodDefinition(QTextCursor cursor);

bool definitionExists(QTextCursor cursor);

MethodDefinitionPattern getMethodDefinitionPatter(QString difinition);
#endif // METHODSPARTSDEFINITIONGETTERS_H
