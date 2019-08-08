#include "palettecreator.h"

AbstractPaletteCreator::~AbstractPaletteCreator()
{
}

DarkPaletteCreator::DarkPaletteCreator(const QPalette &defaultPalette):
    mPalette(defaultPalette)
{
    mPalette.setColor(QPalette::Window, QColor(53,53,53));
    mPalette.setColor(QPalette::WindowText, Qt::white);
    mPalette.setColor(QPalette::Base, QColor("#191919"));
    mPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    mPalette.setColor(QPalette::ToolTipBase, Qt::white);
    mPalette.setColor(QPalette::ToolTipText, Qt::white);
    mPalette.setColor(QPalette::Text, Qt::white);
    mPalette.setColor(QPalette::Button, QColor(53,53,53));
    mPalette.setColor(QPalette::ButtonText, Qt::white);
    mPalette.setColor(QPalette::BrightText, Qt::red);
    mPalette.setColor(QPalette::Disabled, QPalette::Text, QColor("#353535"));
    mPalette.setColor(QPalette::Disabled, QPalette::Light, QColor("#191919"));
    mPalette.setColor(QPalette::Highlight, QColor("#3D7848").lighter());
    mPalette.setColor(QPalette::HighlightedText, QColor("#191919"));
}

QPalette DarkPaletteCreator::getPalette() const
{
    return mPalette;
}
