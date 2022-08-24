//
//  Tile6144.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile6144_h
#define Tile6144_h

#include "Tile.h"

#define TILE6144_ID 14
#define TILE6144_VALUE 6144

class Tile6144 : public Tile
{
public:
    Tile6144();
    virtual ~Tile6144();
    void display() override;
};

#endif /* Tile6144_h */
