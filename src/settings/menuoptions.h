/*
 * The purpose of this class is to show widget with application' settings.
 * Some parametrs just for preview, some for change.
 * There are 5 tabs:
 *  Environment - get access to configuration file; changing application style.
 *  Text editor - with editor' settings: font size, font name.
 *  C++ - with compiler' paths; C++ standart.
 *  Repository - not implemented yet
 *  Connection - with UDP, TCP numbers
*/
#ifndef MENUOPTIONS_H
#define MENUOPTIONS_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class TabEnvironment;
class TabTextEditor;
class TabConnection;
class TabCpp;
class TabRepository;

class QTabWidget;
class QDialogButtonBox;
class QAbstractButton;
QT_END_NAMESPACE

class MenuOptions : public QDialog
{
    Q_OBJECT

public:
    MenuOptions(QWidget *pParent = nullptr);
private:
    QTabWidget *mpTabWidget;

    TabEnvironment *mpTabEnvironment;
    TabTextEditor *mpTabTextEditor;
    TabCpp *mpTabCpp;
    TabRepository *mpTabRepository;
    TabConnection *mpTabConnection;

    QDialogButtonBox *mpButtonBox;

                                                  // map keep changed values
    std::map<QString, QString> mNewValuesToChange;// has fields: "editorFontSize" "editorFont" "style"
                                                  // "UdpPortNumber" "TcpPortNumber" "cppStandart"

    void onBtnApplyClicked();

    // design parameters
    int mMenuWidth              = 800;
    int mMenuHeight             = 280;
private slots:
    void onBtnClicked(QAbstractButton * button);
signals:
    void valuesChanged(std::map<QString, QString> newValues);
};

#endif // MENUOPTIONS_H
