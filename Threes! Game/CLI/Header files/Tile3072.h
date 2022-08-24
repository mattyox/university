//
//  Tile3072.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile3072_h
#define Tile3072_h

#include "Tile.h"

#define TILE3072_ID 13
#define TILE3072_VALUE 3072

class Tile3072 : public Tile
{
public:
    Tile3072();
    virtual ~Tile3072();
    void display() override;
};

#endif /* Tile3072_h */
