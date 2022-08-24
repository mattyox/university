#ifndef Game_h
#define Game_h

#include "Board.h"
#include "Files.h"

#define WINDOW_TITLE "Threes! Project"
#define SCORE_PLACE_SIZE 100
#define FONT_SIZE 45
#define FONT_COLOUR "white"

class Game
{
private:
    // Variable for storing a Board instance
    Board *board;
    // Variable for storing a integer score instance
    int *score;
    // Variable for storing a SFMLGraphics instance
    SFMLGraphics *graphic;
    // Variable for storing a SFMLEvents instance
    SFMLEvents *event;
    
    void show();
    void interact();
    void play();
    void getScore();
    void displayScore();
    void saveScore();
    void waitForClosingWindow();
    
public:
    Game();
    void run();
    ~Game();
};

#endif /* Game_h */
