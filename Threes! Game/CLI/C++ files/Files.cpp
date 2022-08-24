//
//  Files.cpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#include <iostream>
#include <fstream>

#include "Files.h"

void Files :: saveScore(int score)
{
    std::ofstream scoreFile;
    
    scoreFile.open("gameScore.txt");
    
    if(!scoreFile)
    {
        std::cerr << "Cannot open file containing score!" << std::endl;
        return;
    }
    else
    {
        scoreFile << "Score: " << score << std::endl;
        
        scoreFile.close();
    }
}
