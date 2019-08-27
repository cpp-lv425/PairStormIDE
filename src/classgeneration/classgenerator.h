#ifndef CLASSGENERATOR_H
#define CLASSGENERATOR_H

#include "filemanager.h"
#include <QWidget>
#include <QTextCursor>
#include<QPlainTextEdit>

const int indexOfClassNameInLine    = 1;
const int indexOfMethodTypeInLine   = 0;
const int indexOfMethodNameInLine   = 1;
const int leftPartOfDefinitionIndex = 0;

namespace Ui
{
    class ClassGenerator;
}

class ClassGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit ClassGenerator(QWidget *parent = nullptr);
    ~ClassGenerator();

    QString className() const;
    void setClassName(const QString &className);

    QString headerName() const;
    void setHeaderName(const QString &headerName);

    QString sourceCodeName() const;
    void setSourceCodeName(const QString &sourceCodeName);

    friend bool definitionExists(QTextCursor cursor);

private slots:
    void on_OkButton_clicked();

private:
    void setFilesNames();
    bool isValidClassName();
    void setAllFieldsFromUi();
    QString createHeaderMacrosName();
    QString createClassBones();
    QString createHeaderText();
    QString createSourceText();

private:
    Ui::ClassGenerator *ui;
    FileManager fileManager;
    QString mClassName;
    QString mHeaderName;
    QString mSourceCodeName;

};

QString removeExtension(QString str, const int extensionCharacterCount);
QString createMethodDefinitionBones(const QString &dataType,  const QString &className,
                                    const QString &methodName,const QString &methodParams);
QString getTextByCursor(QTextCursor cursor);

QPair<QString, QString> getMethodNameFromFullDefinition(QString definition);
QString getMethodParametrsFromFullDefinition(QString definition);

QString getClassNameForMethodDefinition(QTextCursor cursor);

#endif // CLASSGENERATOR_H
