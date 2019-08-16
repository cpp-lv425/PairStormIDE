#ifndef PALETTECONFIGURATOR_H
#define PALETTECONFIGURATOR_H

#include <QPalette>
#include <QMap>

class QPalette;
class QString;

class PaletteConfigurator
{    
    QMap<QString, QPalette> palettePrototypes;
public:
    explicit PaletteConfigurator(const QPalette &defaultPalette);
    QPalette getPalette(const QString& paletteStyle)const;
};

#endif // PALETTECONFIGURATOR_H
