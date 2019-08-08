#ifndef PALETTECREATOR_H
#define PALETTECREATOR_H

#include <QPalette>

class AbstractPaletteCreator
{
public:    
    virtual QPalette getPalette()const = 0;
    virtual ~AbstractPaletteCreator();
};

class DarkPaletteCreator: public AbstractPaletteCreator
{
    QPalette mPalette;
public:
    DarkPaletteCreator(const QPalette &defaultPalette);
    virtual QPalette getPalette() const;
};


#endif // PALETTECREATOR_H
