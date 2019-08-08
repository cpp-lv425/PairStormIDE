#include "paletteconfigurator.h"

#include "palettecreator.h"

PaletteConfigurator::PaletteConfigurator(const QPalette &defaultPalette)
{
    palettePrototypes.insert("WHITE", defaultPalette);
    palettePrototypes.insert("DARK", DarkPaletteCreator(defaultPalette).getPalette());

}

const QPalette& PaletteConfigurator::getPalette(const QString &paletteStyle) const
{
    return palettePrototypes[paletteStyle];
}

