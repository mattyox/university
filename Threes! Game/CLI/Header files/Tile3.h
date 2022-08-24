//
//  Tile3.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile3_h
#define Tile3_h

#include "Tile.h"

#define TILE3_ID 3
#define TILE3_VALUE 3

class Tile3 : public Tile
{
public:
    Tile3();
    virtual ~Tile3();
    void display() override;
};

#endif /* Tile3_h */
