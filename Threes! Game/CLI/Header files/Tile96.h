//
//  Tile96.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile96_h
#define Tile96_h

#include "Tile.h"

#define TILE96_ID 8
#define TILE96_VALUE 96

class Tile96 : public Tile
{
public:
    Tile96();
    virtual ~Tile96();
    void display() override;
};

#endif /* Tile96_h */
