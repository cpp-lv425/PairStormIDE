#ifndef MENUOPTIONS_H
#define MENUOPTIONS_H

#include "tabenvironment.h"
#include "tabtexteditor.h"
#include "tabconnection.h"
#include "tabcpp.h"
#include "tabrepository.h"

#include <QDialog>
#include <QTabWidget>
#include <QDialogButtonBox>

class MenuOptions : public QDialog
{
    Q_OBJECT

public:
    MenuOptions(QWidget *pParent = nullptr);
private:
    QTabWidget *tabWidget;

    TabEnvironment * tabEnvironment;
    TabTextEditor * tabTextEditor;
    TabCpp * tabCpp;
    TabRepository * tabRepository;
    TabConnection * tabConnection;

    //QDialogButtonBox *buttonBox;
};

#endif // MENUOPTIONS_H
