#include "Game.h"

// Method to displaying the board and tiles
void Game :: show()
{
    graphic->graphicsClear();
    board->displayGrid(graphic);
    board->displayTiles(graphic);
    graphic->graphicsDisplay();
}

// Method to performing a move or closing the window accoring to the pressed key
void Game :: interact()
{
    while(graphic->eventsProcessEvent(*event))
    {
        if (event->getWindowClosed())
        {
            graphic->graphicsClose();
        }
        else if (event->getUpKeyPressed())
        {
            board->moveUp();
        }
        else if (event->getDownKeyPressed())
        {
            board->moveDown();
        }
        else if (event->getRightKeyPressed())
        {
            board->moveRight();
        }
        else if (event->getLeftKeyPressed())
        {
            board->moveLeft();
        }
    }
}

// Method, which hanldes displaying and interacting with the game
void Game :: play()
{
    show();
        
    interact();
}

// Method for getting the score, with use Board method
void Game :: getScore()
{
    *score = board->calculatePoints();
}

// Method for displaying the score
void Game :: displayScore()
{
    graphic->graphicsClear();
    board->displayGrid(graphic);
    board->displayTiles(graphic);
    // Method from SFMLGraphics, which draws the score text at the center of score place (at window bottom)
    graphic->graphicsDrawText("Score: " + std::to_string(*score), FONT_SIZE, FONT_COLOUR, ((HORIZONTAL_BOARD_SIZE / 2) * (GRID_SIZE + BOARDER_SIZE)), ((VERTICAL_BOARD_SIZE * (GRID_SIZE + BOARDER_SIZE)) + SCORE_PLACE_SIZE / 2));
    graphic->graphicsDisplay();
}

// Method for saving the score, with use of Files class
void Game :: saveScore()
{
    Files scoreFile;
    
    scoreFile.saveScore(*score);
}

// Method, which stays in loop till window is closed
void Game :: waitForClosingWindow()
{
    while (graphic->graphicsWindowIsOpen())
    {
        while(graphic->eventsProcessEvent(*event))
        {
            if (event->getWindowClosed())
            {
                graphic->graphicsClose();
            }
        }
    }
}

//  Default constructor, which initializes instances of Board, integer score, SFMLGraphics (with defined parameters) and SFMLEvents
Game :: Game()
{
    board = new Board();
    score = new int(0);
    graphic = new SFMLGraphics();
    graphic->graphicsInitialize((HORIZONTAL_BOARD_SIZE * (GRID_SIZE + BOARDER_SIZE + 2)), ((VERTICAL_BOARD_SIZE * (GRID_SIZE + BOARDER_SIZE)) + SCORE_PLACE_SIZE), WINDOW_TITLE);
    event = new SFMLEvents();
}

// Method for playing the game till window is open and grid is not full, then it gets the score, displays it, saves it and waits for closing window
void Game :: run()
{
    while ((graphic->graphicsWindowIsOpen()) && (!board->isGridFull()))
    {
        play();
    }
    
    getScore();
    
    displayScore();
    
    saveScore();
    
    waitForClosingWindow();
}

// Default destructor for deleting all pointers stored in the class
Game :: ~Game()
{
    delete board;
    delete score;
    delete event;
    delete graphic;
}
