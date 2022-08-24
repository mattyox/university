//
//  Tile2.hpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile2_hpp
#define Tile2_hpp

#include "Tile.h"

#define TILE2_ID 2
#define TILE2_VALUE 2

class Tile2 : public Tile
{
public:
    Tile2();
    virtual ~Tile2();
    void display() override;
};

#endif /* Tile2_hpp */
