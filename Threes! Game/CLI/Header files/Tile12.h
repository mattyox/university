//
//  Tile12.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile12_h
#define Tile12_h

#include "Tile.h"

#define TILE12_ID 5
#define TILE12_VALUE 12

class Tile12 : public Tile
{
public:
    Tile12();
    virtual ~Tile12();
    void display() override;
};

#endif /* Tile12_h */
