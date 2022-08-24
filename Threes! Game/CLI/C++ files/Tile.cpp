//
//  Tile.cpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#include "Tile.h"

Tile :: Tile(int i, int v) : idNumber(i), value(v)
{
    ;
}

Tile :: ~Tile()
{
    ;
}

int Tile :: getIDNumber()
{
    return idNumber;
}

int Tile :: getPoints()
{
    return value * value;
}
