#include "settingsconfigurator.h"

#include "mainwindow.h"

SettingsConfigurator::SettingsConfigurator()
{    
    settingsFunctors.insert("style", std::function<void(MainWindow*, QString)>
                            (&MainWindow::setAppStyle));
    settingsFunctors.insert("editorFont", std::function<void(MainWindow*, QString)>
                            (&MainWindow::setDocumentFontFamily));
    settingsFunctors.insert("editorFontSize", std::function<void(MainWindow*, QString)>
                            (&MainWindow::setDocumentFontSize));
}

std::function<void(MainWindow*, QString)> SettingsConfigurator::getSettingsFunctor(const QString &setting) const
{
    return settingsFunctors[setting];
}
