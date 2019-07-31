#ifndef DOCUMENTATIONENGINE_H
#define DOCUMENTATIONENGINE_H

#include <QUrl>
#include <QObject>
#include <QVector>
#include <QFileInfo>

class DocumentationWebEngine : public QObject
{
    Q_OBJECT
public:
    explicit DocumentationWebEngine(QObject *parent = nullptr);
    void searchByKeyword(const QString &keyword);
    void searchByLibraryAndFunction(const QString &include, const QString &function);
signals:

public slots:

private:
    QVector<QUrl> mDocumentationLinks;
};

#endif // DOCUMENTATIONENGINE_H
