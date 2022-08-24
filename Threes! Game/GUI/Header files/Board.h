#ifndef Board_h
#define Board_h

#include <ctime>
#include <cstdlib>

#include "Tile1.h"
#include "Tile2.h"
#include "Tile3.h"
#include "Tile6.h"
#include "Tile12.h"
#include "Tile24.h"
#include "Tile48.h"
#include "Tile96.h"
#include "Tile192.h"
#include "Tile384.h"
#include "Tile768.h"
#include "Tile1536.h"
#include "Tile3072.h"
#include "Tile6144.h"

#define VERTICAL_BOARD_SIZE 4
#define HORIZONTAL_BOARD_SIZE 4

#include "SFMLGraphics.h"

#define GRID_SIZE 128
#define BOARDER_SIZE 8
#define INSIDE_COLOUR "black"
#define OUTSIDE_COLOUR "white"

class Board
{
private:
    // Variables for storing dimensions of the board
    int height, width, size, border;
    // Variable (2D std::vector) storing pointers to Tile
    std::vector<std::vector<Tile*>> grid;
    // std::vector for storing the all tiles id numbers and only used tiles id numbers
    std::vector<int> allTypes, usedTypes;
    bool gridIsFull;
    // Variable for storing inside and outside colours of one board slot
    std::string insideColour, outsideColour;
    
    std::vector<std::vector<Tile*>> initializeGrid();
    std::vector<int> initializeAllTypes();
    std::vector<int> initializeUsedTypes();
    
    void moveTilesUp();
    std::vector<int> searchForFreeBoxInLastRow();
    void moveTilesDown();
    std::vector<int> searchForFreeBoxInFirstRow();
    void moveTilesRight();
    std::vector<int> searchForFreeBoxInFirstColumn();
    void moveTilesLeft();
    std::vector<int> searchForFreeBoxInLastColumn();
    
    Tile* addTile();
    Tile* mergeTiles(int typeNumber);
    Tile* createTile(int typeNumber);
    void updateUsedTypes(int typeNumber);
    
public:
    Board();
    ~Board();
    
    void displayGrid(SFMLGraphics *graphic);
    void displayTiles(SFMLGraphics *graphic);
    
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    
    bool isGridFull();
    
    int calculatePoints();
};

#endif /* Board_h */
