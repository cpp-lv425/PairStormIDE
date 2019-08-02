/*
 * The purpose of this class is to show widget with application' settings
 * some parametrs just for preview either for change
 * there are 5 tabs:
 *  Environment - get access to configuration file,
 *  Text editor - with editor' settings: style, font size, font name,
 *  C++ - with compiler settings, paths
 *  Repository - with
 *  Connection - with
*/
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

    QDialogButtonBox *buttonBox;

    void onBtnApplyClicked();
private slots:
    void onBtnClicked(QAbstractButton * button);
signals:
    void valuesForEditor(std::map<QString, QString> newValues);
};

#endif // MENUOPTIONS_H
