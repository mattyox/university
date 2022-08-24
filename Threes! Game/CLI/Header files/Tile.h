//
//  Tile.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile_h
#define Tile_h

#include <iostream>

class Tile
{
protected:
    int idNumber;
    int value;
    
public:
    Tile(int i, int v);
    virtual ~Tile();
    int getIDNumber();
    int getPoints();
    virtual void display() = 0;
};

#endif /* Tile_h */
