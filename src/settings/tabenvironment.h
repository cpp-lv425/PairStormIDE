#ifndef TABENVIRONMENT_H
#define TABENVIRONMENT_H

#include <QWidget>

class TabEnvironment : public QWidget
{
    Q_OBJECT
public:
    explicit TabEnvironment(QWidget *parent = nullptr);
private:
    QString currentPath;
signals:

public slots:
    void onClicked();
};

#endif // TABENVIRONMENT_H
