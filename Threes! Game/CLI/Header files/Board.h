//
//  Board.h
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#ifndef Board_h
#define Board_h

#include <vector>
#include <string>
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

class Board
{
private:
    int height, width;
    std::vector<std::vector<Tile*>> grid;
    std::vector<int> allTypes;
    std::vector<int> usedTypes;
    bool gridIsFull;
    
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
    
    void displayBoard();
    
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    
    bool isGridFull();
    
    int calculatePoints();
};

#endif /* Board_h */
