//
//  Game.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Game_h
#define Game_h

#include <iostream>

#include "Board.h"
#include "Files.h"

#define w_CHAR 119
#define s_CHAR 115
#define d_CHAR 100
#define a_CHAR 97

class Game
{
private:
    Board *board;
    int score;
    
    void show();
    void makeMove();
    void getScore();
    void displayScore();
    void saveScore();
    
public:
    void start();
    void play();
    void finish();
};

#endif /* Game_h */
