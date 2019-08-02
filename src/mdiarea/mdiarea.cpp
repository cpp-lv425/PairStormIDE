#include "mdiarea.h"

MDIArea::MDIArea(QWidget *pParent): QMdiArea(pParent)
{
    setViewMode(QMdiArea::TabbedView);
    setTabsClosable(true);
}
