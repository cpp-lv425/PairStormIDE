#ifndef MDIAREA_H
#define MDIAREA_H

#include <QMdiArea>

class MDIArea: public QMdiArea
{
    Q_OBJECT
public:
    MDIArea(QWidget *pParent = nullptr);
};

#endif // MDIAREA_H
