#ifndef CLASSGENERATOR_H
#define CLASSGENERATOR_H

#include <QWidget>

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
private slots:
    void on_OkButton_clicked();

private:
    void setFilesNames();
    bool isValidClassName();
    void setAllFieldsFromUi();
    QString createHeaderMacrosName();
    QString removeExtension(QString str, const int extensionCharacterCount);
    QString createClassBones();
    QString createMethodDefinitionBones(const QString methodName, const QString methodParams);
    QString createHeaderText();
    QString createSourceText();

private:
    Ui::ClassGenerator *ui;
    QString mClassName;
    QString mHeaderName;
    QString mSourceCodeName;
};

#endif // CLASSGENERATOR_H
