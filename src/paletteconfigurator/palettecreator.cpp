#include "palettecreator.h"

AbstractPaletteCreator::~AbstractPaletteCreator()
{
}

DarkPaletteCreator::DarkPaletteCreator(const QPalette &defaultPalette):
    mPalette(defaultPalette)
{
    mPalette.setColor(QPalette::Window, QColor("#353535"));
    mPalette.setColor(QPalette::WindowText, QColor("#DDDDDD"));
    mPalette.setColor(QPalette::Base, QColor("#191919"));
    mPalette.setColor(QPalette::AlternateBase, QColor("#353535"));
    mPalette.setColor(QPalette::ToolTipBase, QColor("#DDDDDD"));
    mPalette.setColor(QPalette::ToolTipText, QColor("#DDDDDD"));
    mPalette.setColor(QPalette::Text, QColor("#DDDDDD"));
    mPalette.setColor(QPalette::Button, QColor("#353535"));
    mPalette.setColor(QPalette::ButtonText, QColor("#DDDDDD"));
    mPalette.setColor(QPalette::BrightText, QColor("#ff0000"));
    mPalette.setColor(QPalette::Disabled, QPalette::Text, QColor("#353535"));
    mPalette.setColor(QPalette::Disabled, QPalette::Light, QColor("#191919"));
    mPalette.setColor(QPalette::Highlight, QColor("#3D7848").lighter());
    mPalette.setColor(QPalette::HighlightedText, QColor("#191919"));
}

QPalette DarkPaletteCreator::getPalette() const
{
    return mPalette;
}
