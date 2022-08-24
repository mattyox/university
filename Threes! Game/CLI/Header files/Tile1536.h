//
//  Tile1536.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile1536_h
#define Tile1536_h

#include "Tile.h"

#define TILE1536_ID 12
#define TILE1536_VALUE 1536

class Tile1536 : public Tile
{
public:
    Tile1536();
    virtual ~Tile1536();
    void display() override;
};

#endif /* Tile1536_h */
