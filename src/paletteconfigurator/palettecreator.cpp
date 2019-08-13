#include "palettecreator.h"

AbstractPaletteCreator::~AbstractPaletteCreator()
{
}

DarkPaletteCreator::DarkPaletteCreator(const QPalette &defaultPalette):
    mPalette(defaultPalette)
{
    //  create colors prototypes
    mColors.insert(Colors::DARK, "#191919");
    mColors.insert(Colors::MEDIUM, "#353535");
    mColors.insert(Colors::MEDLIGHT, "#5A5A5A");
    mColors.insert(Colors::LIGHT, "#DDDDDD");
    mColors.insert(Colors::ACCENT, "#3D7848");

    // configuring colors of active group
    mPalette.setColor(QPalette::Window, mColors[Colors::MEDIUM]);
    mPalette.setColor(QPalette::WindowText, mColors[Colors::LIGHT]);
    mPalette.setColor(QPalette::Base, mColors[Colors::DARK]);
    mPalette.setColor(QPalette::AlternateBase, mColors[Colors::MEDIUM]);
    mPalette.setColor(QPalette::ToolTipBase, mColors[Colors::LIGHT]);
    mPalette.setColor(QPalette::ToolTipText, mColors[Colors::LIGHT]);
    mPalette.setColor(QPalette::Text, mColors[Colors::LIGHT]);
    mPalette.setColor(QPalette::Button, mColors[Colors::MEDIUM]);
    mPalette.setColor(QPalette::ButtonText, mColors[Colors::LIGHT]);
    mPalette.setColor(QPalette::Highlight, mColors[Colors::ACCENT].lighter());
    mPalette.setColor(QPalette::HighlightedText, mColors[Colors::DARK]);

    // configuring colors of disabled group
    mPalette.setColor(QPalette::Disabled, QPalette::Text, mColors[Colors::MEDLIGHT]);
    mPalette.setColor(QPalette::Disabled, QPalette::Light, mColors[Colors::DARK]);
    mPalette.setColor(QPalette::Disabled, QPalette::WindowText, mColors[Colors::MEDLIGHT]);
}

QPalette DarkPaletteCreator::getPalette() const
{
    return mPalette;
}
