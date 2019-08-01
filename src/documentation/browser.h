#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>

#include "mdiarea.h"

class Browser : public QMainWindow
{
    Q_OBJECT
public:
    explicit Browser(QWidget *parent = nullptr);
signals:

public slots:
    void newTab(const QString &keyword);
private:
    MDIArea *mBrowseArea;
};

#endif // BROWSER_H
