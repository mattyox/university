//
//  Tile48.cpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#include "Tile48.h"

Tile48 :: Tile48() : Tile(TILE48_ID, TILE48_VALUE)
{
    ;
}

Tile48 :: ~Tile48()
{
    ;
}

void Tile48 :: display()
{
//    std::cout << "Tile48.jpeg";
    std::cout << "48";
}
