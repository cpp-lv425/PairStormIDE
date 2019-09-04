#ifndef TABREPOSITORY_H
#define TABREPOSITORY_H

#include <QWidget>

class TabRepository : public QWidget
{
    Q_OBJECT
public:
    explicit TabRepository(QWidget *parent = nullptr);
private:
    // design parameters
    int mBasicStretch                = 1;

signals:

public slots:
};

#endif // TABREPOSITORY_H
