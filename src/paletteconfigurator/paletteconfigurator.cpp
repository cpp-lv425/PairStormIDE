#include "paletteconfigurator.h"

#include "palettecreator.h"

PaletteConfigurator::PaletteConfigurator(const QPalette &defaultPalette)
{
    // container is populated with available palette prototypes
    palettePrototypes.insert("WHITE", defaultPalette);
    palettePrototypes.insert("DARK", DarkPaletteCreator(defaultPalette).getPalette());
}

const QPalette& PaletteConfigurator::getPalette(const QString &paletteStyle) const
{
    return palettePrototypes[paletteStyle];
}

