#ifndef Files_h
#define Files_h

#include "Exceptions.h"

#define GAMESCORE_FILE "gameScore.txt"

// Class used for operation on files
class Files
{
public:
    void saveScore(int score);
};

#endif /* Files_h */
