//
//  Tile1.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile1_h
#define Tile1_h

#include "Tile.h"

#define TILE1_ID 1
#define TILE1_VALUE 1

class Tile1 : public Tile
{
public:
    Tile1();
    virtual ~Tile1();
    void display() override;
};

#endif /* Tile1_h */
