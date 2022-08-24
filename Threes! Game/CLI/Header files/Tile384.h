//
//  Tile384.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile384_h
#define Tile384_h

#include "Tile.h"

#define TILE384_ID 10
#define TILE384_VALUE 384

class Tile384 : public Tile
{
public:
    Tile384();
    virtual ~Tile384();
    void display() override;
};

#endif /* Tile384_h */
