//
//  Tile48.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile48_h
#define Tile48_h

#include "Tile.h"

#define TILE48_ID 7
#define TILE48_VALUE 48

class Tile48 : public Tile
{
public:
    Tile48();
    virtual ~Tile48();
    void display() override;
};

#endif /* Tile48_h */
