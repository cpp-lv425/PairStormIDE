#ifndef METHODSPARTSDEFINITIONGETTERS_H
#define METHODSPARTSDEFINITIONGETTERS_H

#include<QString>
#include<QTextCursor>

QString removeExtension(QString str, const int extensionCharacterCount);
QString createMethodDefinitionBones(const QString &dataType,  const QString &className,
                                    const QString &methodName,const QString &methodParams);
QString getTextByCursor(QTextCursor cursor);

QPair<QString, QString> getMethodNameFromFullDefinition(QString definition);
QString getMethodParametrsFromFullDefinition(QString definition);

QString getClassNameForMethodDefinition(QTextCursor cursor);

bool definitionExists(QTextCursor cursor);

#endif // METHODSPARTSDEFINITIONGETTERS_H
