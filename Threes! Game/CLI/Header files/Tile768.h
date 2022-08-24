//
//  Tile768.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Tile768_h
#define Tile768_h

#include "Tile.h"

#define TILE768_ID 11
#define TILE768_VALUE 768

class Tile768 : public Tile
{
public:
    Tile768();
    virtual ~Tile768();
    void display() override;
};

#endif /* Tile768_h */
