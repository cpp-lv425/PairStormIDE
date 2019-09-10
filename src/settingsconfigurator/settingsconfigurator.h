#ifndef SETTINGSCONFIGURATOR_H
#define SETTINGSCONFIGURATOR_H

#include <QMap>

class MainWindow;

class SettingsConfigurator
{    
    QMap<QString, std::function<void(MainWindow*, QString)>> settingsFunctors;
public:
    SettingsConfigurator();
    std::function<void(MainWindow*, QString)> getSettingsFunctor(const QString &setting) const;
};

#endif // SETTINGSCONFIGURATOR_H
