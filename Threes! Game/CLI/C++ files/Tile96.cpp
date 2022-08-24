//
//  Tile96.cpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#include "Tile96.h"

Tile96 :: Tile96() : Tile(TILE96_ID, TILE96_VALUE)
{
    ;
}

Tile96 :: ~Tile96()
{
    ;
}

void Tile96 :: display()
{
//    std::cout << "Tile96.jpeg";
    std::cout << "96";
}
