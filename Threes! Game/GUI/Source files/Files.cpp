#include <iostream>
#include <fstream>

#include "Files.h"

// Method for saving the obtained score to GAMESCORE_FILE
void Files :: saveScore(int score)
{
    try
    {
        std::ofstream scoreFile;
        
        scoreFile.open(GAMESCORE_FILE);
        
        // When scoreFile is openned it simply writes the into it and closes it
        if (scoreFile)
        {
            scoreFile << "Score: " << score << std::endl;
            
            scoreFile.close();
        }
        // Otherwise an exception is thrown
        else
        {
            throw Exceptions(GAMESCORE_FILE);
        }
    }
    // The catched expection is printed to standart error stream
    catch (Exceptions &e)
    {
        std::cerr << e.what();
        return;
    }
}
