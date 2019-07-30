#ifndef DOCUMENTATIONSEARCH_H
#define DOCUMENTATIONSEARCH_H

#include <QObject>

class DocumentationSearch : public QObject
{
    Q_OBJECT
public:
    explicit DocumentationSearch(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DOCUMENTATIONSEARCH_H
