//
//  Tile6.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile6_h
#define Tile6_h

#include "Tile.h"

#define TILE6_ID 4
#define TILE6_VALUE 6

class Tile6 : public Tile
{
public:
    Tile6();
    virtual ~Tile6();
    void display() override;
};

#endif /* Tile6_h */
