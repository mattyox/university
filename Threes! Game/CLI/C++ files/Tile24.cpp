//
//  Tile24.cpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#include "Tile24.h"

Tile24 :: Tile24() : Tile(TILE24_ID, TILE24_VALUE)
{
    ;
}

Tile24 :: ~Tile24()
{
    ;
}

void Tile24 :: display()
{
//    std::cout << "Tile24.jpeg";
    std::cout << "24";
}
