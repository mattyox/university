//
//  Tile6144.cpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#include "Tile6144.h"

Tile6144 :: Tile6144() : Tile(TILE6144_ID, TILE6144_VALUE)
{
    ;
}

Tile6144 :: ~Tile6144()
{
    ;
}

void Tile6144 :: display()
{
//    std::cout << "Tile6144.jpeg";
    std::cout << "6144";
}
