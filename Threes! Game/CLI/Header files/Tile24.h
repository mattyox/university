//
//  Tile24.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile24_h
#define Tile24_h

#include "Tile.h"

#define TILE24_ID 6
#define TILE24_VALUE 24

class Tile24 : public Tile
{
public:
    Tile24();
    virtual ~Tile24();
    void display() override;
};

#endif /* Tile24_h */
