//
//  Game.cpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#include "Game.h"

void Game :: show()
{
    board->displayBoard();
}

void Game :: makeMove()
{
    char key;
    
    std::cin >> key;
    
    switch (key) {
        case w_CHAR:
            board->moveUp();
            break;
            
        case s_CHAR:
            board->moveDown();
            break;
            
        case d_CHAR:
            board->moveRight();
            break;
            
        case a_CHAR:
            board->moveLeft();
            break;
            
        default:
            break;
    }
}

void Game :: getScore()
{
    score = board->calculatePoints();
}

void Game :: displayScore()
{
    std::cout << "\nScore: " << score << "\n\n";
}

void Game :: saveScore()
{
    Files scoreFile;
    
    scoreFile.saveScore(score);
}

void Game :: start()
{
    board = new Board();
    
    score = 0;
}

void Game :: play()
{
    while (!board->isGridFull())
    {
        show();
        makeMove();
    }
    
    getScore();
    
    displayScore();
    
    saveScore();
}

void Game :: finish()
{
    delete board;
}
