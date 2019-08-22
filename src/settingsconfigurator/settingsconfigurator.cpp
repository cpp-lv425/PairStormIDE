#include "settingsconfigurator.h"

#include "mainwindow.h"

SettingsConfigurator::SettingsConfigurator()
{    
    settingsFunctors.insert("style", std::function<void(MainWindow*, QString)>
                            (&MainWindow::setAppStyle));
}

std::function<void(MainWindow*, QString)> SettingsConfigurator::getSettingsFunctor(const QString &setting) const
{
    return settingsFunctors[setting];
}
