//
//  Tile192.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile192_h
#define Tile192_h

#include "Tile.h"

#define TILE192_ID 9
#define TILE192_VALUE 192

class Tile192 : public Tile
{
public:
    Tile192();
    virtual ~Tile192();
    void display() override;
};

#endif /* Tile192_h */
