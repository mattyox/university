#include "Tile.h"

// Constructor of Tile class, which assigns value i to idNumber variable and value v to value variable
Tile :: Tile(int i, int v) : idNumber(i), value(v)
{
    ;
}

// Returns the idNumber of Tile
int Tile :: getIDNumber()
{
    return idNumber;
}

// Returns the points (squared value) of Tile
int Tile :: getPoints()
{
    return value * value;
}
