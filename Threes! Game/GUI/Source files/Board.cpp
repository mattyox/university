#include "Board.h"

// Method for preparing a grid variable with nullptrs, according to the given dimensions
std::vector<std::vector<Tile*>> Board :: initializeGrid()
{
    std::vector<std::vector<Tile*>> initialGrid;

    for (int i = 1; i <= height; i++)
    {
        initialGrid.push_back(std::vector<Tile*>(width, nullptr));
    }

    return initialGrid;
}

// Method for preparing a std::vector of all tiles id numbers
std::vector<int> Board :: initializeAllTypes()
{
    std::vector<int> initialAllTypes;
    
    initialAllTypes.push_back(Tile1().getIDNumber());
    initialAllTypes.push_back(Tile2().getIDNumber());
    initialAllTypes.push_back(Tile3().getIDNumber());
    initialAllTypes.push_back(Tile6().getIDNumber());
    initialAllTypes.push_back(Tile12().getIDNumber());
    initialAllTypes.push_back(Tile24().getIDNumber());
    initialAllTypes.push_back(Tile48().getIDNumber());
    initialAllTypes.push_back(Tile96().getIDNumber());
    initialAllTypes.push_back(Tile192().getIDNumber());
    initialAllTypes.push_back(Tile384().getIDNumber());
    initialAllTypes.push_back(Tile768().getIDNumber());
    initialAllTypes.push_back(Tile1536().getIDNumber());
    initialAllTypes.push_back(Tile3072().getIDNumber());
    initialAllTypes.push_back(Tile6144().getIDNumber());
    
    return initialAllTypes;
}

// Method for preparing a std::vector of used tiles id numbers. It has initial values as according to rules, the game starts with Tile1 and Tile2 avaible
std::vector<int> Board :: initializeUsedTypes()
{
    std::vector<int> initialUsedTypes;
    
    initialUsedTypes.push_back(Tile1().getIDNumber());
    initialUsedTypes.push_back(Tile2().getIDNumber());
    
    return initialUsedTypes;
}

// Method for moving all Tiles up. It stars to iterate from the second row
void Board :: moveTilesUp()
{
    for (int i = 1; i <= (height - 1); i++)
    {
        for (int j = 0; j <= (width - 1); j++)
        {
            // When both pointers are nullptrs
            if (((grid[i - 1][j] == nullptr) && (grid[i][j] == nullptr)))
            {
                ; // Do nothing
            }
            // When the former pointer nullptr and the current is not nullptr
            else if ((grid[i - 1][j] == nullptr) && (grid[i][j] != nullptr))
            {
                // Simply move tile, by deleting old one and creating new at former position
                grid[i - 1][j] = createTile(grid[i][j]->getIDNumber());
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
            // When the former pointer is not nullptr and the current is
            else if ((grid[i - 1][j] != nullptr) && (grid[i][j] == nullptr))
            {
                ; // Do nothing
            }
            // When objects are both Tile1, Tile2 or Tile6144
            else if (((grid[i - 1][j]->getIDNumber() == Tile6144().getIDNumber()) && (grid[i][j]->getIDNumber() == Tile6144().getIDNumber())) || ((grid[i - 1][j]->getIDNumber() == Tile1().getIDNumber()) && (grid[i][j]->getIDNumber() == Tile1().getIDNumber())) || ((grid[i - 1][j]->getIDNumber() == Tile2().getIDNumber()) && (grid[i][j]->getIDNumber() == Tile2().getIDNumber())))
            {
                ; // Do nothing
            }
            // When one object is Tile1 and other is Tile2 or one is Tile2 and the other os Tile1
            else if (((grid[i - 1][j]->getIDNumber() == Tile1().getIDNumber()) && ((grid[i][j])->getIDNumber() == Tile2().getIDNumber())) || ((grid[i - 1][j]->getIDNumber() == Tile2().getIDNumber()) && ((grid[i][j])->getIDNumber() == Tile1().getIDNumber())))
            {
                // Delete former pointer
                delete grid[i - 1][j];
                // In this place creates a new Tile3
                grid[i - 1][j] = createTile(Tile3().getIDNumber());
                // If it is necessary update usedTypes vector
                if (usedTypes.size() == 2) usedTypes.push_back(Tile3().getIDNumber());
                // Delete the current pointer
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
            // When both object are of the same type
            else if ((grid[i - 1][j]->getIDNumber()) == ((grid[i][j]->getIDNumber())))
            {
                // Delete former pointer
                delete grid[i - 1][j];
                // In this place creates a new Tile of higher type
                grid[i - 1][j] = mergeTiles(grid[i][j]->getIDNumber());
                // Delete the current pointer
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
    }
}

// Method for searching free boxes in last row of the grid
std::vector<int> Board :: searchForFreeBoxInLastRow()
{
    std::vector<int> freeBoxesInLastRow;
    
    for (int j = 0; j <= (width - 1); j++)
    {
        if (grid[height - 1][j] == nullptr)
        {
            freeBoxesInLastRow.push_back(j);
        }
    }
    
    return freeBoxesInLastRow;
}

// Method for moving all Tiles up. It stars to iterate from the next-to-last row
void Board :: moveTilesDown()
{
    for (int i = (height - 2); i >= 0; i--)
    {
        for (int j = 0; j <= (width - 1); j++)
        {
            // When both pointers are nullptrs
            if (((grid[i][j] == nullptr) && (grid[i + 1][j] == nullptr)))
            {
                ; // Do nothing
            }
            // When the current pointer nullptr and the next is not nullptr
            else if ((grid[i][j] == nullptr) && (grid[i + 1][j] != nullptr))
            {
                ; // Do nothing
            }
            // When the current pointer is not nullptr and the next is
            else if ((grid[i][j] != nullptr) && (grid[i + 1][j] == nullptr))
            {
                // Simply move tile, by deleting old one and creating new at next position
                grid[i + 1][j] = createTile(grid[i][j]->getIDNumber());
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
            // When objects are both Tile1, Tile2 or Tile6144
            else if (((grid[i][j]->getIDNumber() == Tile6144().getIDNumber()) && (grid[i + 1][j]->getIDNumber() == Tile6144().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && (grid[i + 1][j]->getIDNumber() == Tile1().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && (grid[i + 1][j]->getIDNumber() == Tile2().getIDNumber())))
            {
                ; // Do nothing
            }
            // When one object is Tile1 and other is Tile2 or one is Tile2 and the other os Tile1
            else if (((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && ((grid[i + 1][j])->getIDNumber() == Tile2().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && ((grid[i + 1][j])->getIDNumber() == Tile1().getIDNumber())))
            {
                // Delete next pointer
                delete grid[i + 1][j];
                // In this place creates a new Tile3
                grid[i + 1][j] = createTile(Tile3().getIDNumber());
                // If it is necessary update usedTypes vector
                if (usedTypes.size() == 2) usedTypes.push_back(Tile3().getIDNumber());
                // Delete the current pointer
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
            // When both object are of the same type
            else if ((grid[i][j]->getIDNumber()) == ((grid[i + 1][j]->getIDNumber())))
            {
                // Delete next pointer
                delete grid[i + 1][j];
                // In this place creates a new Tile of higher type
                grid[i + 1][j] = mergeTiles(grid[i][j]->getIDNumber());
                // Delete the current pointer
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
    }
}

// Method for searching free boxes in first row of the grid
std::vector<int> Board :: searchForFreeBoxInFirstRow()
{
    std::vector<int> freeBoxesInFirstRow;
    
    for (int j = 0; j <= (width - 1); j++)
    {
        if (grid[0][j] == nullptr)
        {
            freeBoxesInFirstRow.push_back(j);
        }
    }
    
    return freeBoxesInFirstRow;
}

// Method for moving all Tiles up. It stars to iterate from the last column
void Board :: moveTilesRight()
{
    for (int i = 0; i <= (height - 1); i++)
    {
        for (int j = (width - 1); j >= 1; j--)
        {
            // When both pointers are nullptrs
            if (((grid[i][j - 1] == nullptr) && (grid[i][j] == nullptr)))
            {
                ; // Do nothing
            }
            // When the former pointer nullptr and the current is not nullptr
            else if ((grid[i][j - 1] == nullptr) && (grid[i][j] != nullptr))
            {
                ; // Do nothing
            }
            // When the current pointer is not nullptr and the next is
            else if ((grid[i][j - 1] != nullptr) && (grid[i][j] == nullptr))
            {
                // Simply move tile, by deleting old one and creating new at current position
                grid[i][j] = createTile(grid[i][j - 1]->getIDNumber());
                delete grid[i][j - 1];
                grid[i][j - 1] = nullptr;
            }
            // When objects are both Tile1, Tile2 or Tile6144
            else if (((grid[i][j - 1]->getIDNumber() == Tile6144().getIDNumber()) && (grid[i][j]->getIDNumber() == Tile6144().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && (grid[i][j - 1]->getIDNumber() == Tile1().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && (grid[i][j - 1]->getIDNumber() == Tile2().getIDNumber())))
            {
                ; // Do nothing
            }
            // When one object is Tile1 and other is Tile2 or one is Tile2 and the other os Tile1
            else if (((grid[i][j - 1]->getIDNumber() == Tile1().getIDNumber()) && ((grid[i][j])->getIDNumber() == Tile2().getIDNumber())) || ((grid[i][j - 1]->getIDNumber() == Tile2().getIDNumber()) && ((grid[i][j])->getIDNumber() == Tile1().getIDNumber())))
            {
                // Delete current pointer
                delete grid[i][j];
                // In this place creates a new Tile3
                grid[i][j] = createTile(Tile3().getIDNumber());
                // If it is necessary update usedTypes vector
                if (usedTypes.size() == 2) usedTypes.push_back(Tile3().getIDNumber());
                // Delete the former pointer
                delete grid[i][j - 1];
                grid[i][j - 1] = nullptr;
            }
            // When both object are of the same type
            else if ((grid[i][j - 1]->getIDNumber()) == ((grid[i][j]->getIDNumber())))
            {
                // Delete current pointer
                delete grid[i][j];
                // In this place creates a new Tile of higher type
                grid[i][j] = mergeTiles(grid[i][j - 1]->getIDNumber());
                // Delete the former pointer
                delete grid[i][j - 1];
                grid[i][j - 1] = nullptr;
            }
        }
    }
}

// Method for searching free boxes in first column of the grid
std::vector<int> Board :: searchForFreeBoxInFirstColumn()
{
    std::vector<int> freeBoxesInFirstColumn;
    
    for (int i = 0; i <= (height - 1); i++)
    {
        if (grid[i][0] == nullptr)
        {
            freeBoxesInFirstColumn.push_back(i);
        }
    }
    
    return freeBoxesInFirstColumn;
}

// Method for moving all Tiles up. It stars to iterate from the first column
void Board :: moveTilesLeft()
{
    for (int i = 0; i <= (height - 1); i++)
    {
        for (int j = 0; j <= (width - 2); j++)
        {
            // When both pointers are nullptrs
            if (((grid[i][j] == nullptr) && (grid[i][j + 1] == nullptr)))
            {
                ; // Do nothing
            }
            // When the current pointer nullptr and the next is not nullptr
            else if ((grid[i][j] == nullptr) && (grid[i][j + 1] != nullptr))
            {
                // Simply move tile, by deleting old one and creating new at next position
                grid[i][j] = createTile(grid[i][j + 1]->getIDNumber());
                delete grid[i][j + 1];
                grid[i][j + 1] = nullptr;
            }
            // When the current pointer is not nullptr and the next is
            else if ((grid[i][j] != nullptr) && (grid[i][j + 1] == nullptr))
            {
                ; // Do nothing
            }
            // When objects are both Tile1, Tile2 or Tile6144
            else if (((grid[i][j]->getIDNumber() == Tile6144().getIDNumber()) && (grid[i][j + 1]->getIDNumber() == Tile6144().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && (grid[i][j + 1]->getIDNumber() == Tile1().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && (grid[i][j + 1]->getIDNumber() == Tile2().getIDNumber())))
            {
                ; // Do nothing
            }
            // When one object is Tile1 and other is Tile2 or one is Tile2 and the other os Tile1
            else if (((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && ((grid[i][j + 1])->getIDNumber() == Tile2().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && ((grid[i][j + 1])->getIDNumber() == Tile1().getIDNumber())))
            {
                // Delete current pointer
                delete grid[i][j];
                // In this place creates a new Tile3
                grid[i][j] = createTile(Tile3().getIDNumber());
                // If it is necessary update usedTypes vector
                if (usedTypes.size() == 2) usedTypes.push_back(Tile3().getIDNumber());
                // Delete the next pointer
                delete grid[i][j + 1];
                grid[i][j + 1] = nullptr;
            }
            // When both object are of the same type
            else if ((grid[i][j]->getIDNumber()) == ((grid[i][j + 1]->getIDNumber())))
            {
                // Delete current pointer
                delete grid[i][j];
                // In this place creates a new Tile of higher type
                grid[i][j] = mergeTiles(grid[i][j + 1]->getIDNumber());
                // Delete the next pointer
                delete grid[i][j + 1];
                grid[i][j + 1] = nullptr;
            }
        }
    }
}

// Method for searching free boxes in last column of the grid
std::vector<int> Board :: searchForFreeBoxInLastColumn()
{
    std::vector<int> freeBoxesInLastColumn;
    
    for (int i = 0; i <= (height - 1); i++)
    {
        if (grid[i][width - 1] == nullptr)
        {
            freeBoxesInLastColumn.push_back(i);
        }
    }
    
    return freeBoxesInLastColumn;
}

// Method for adding Tiles to the grid
Tile* Board :: addTile()
{
    int usedTypesNumber = int(usedTypes.size());
    
    // Draws a random number according to the usedTypes vector lenght
    int randomTypeNumber = (rand() % usedTypesNumber) + 1;
    
    // Creates appropriate Tile
    Tile* createdTile = createTile(randomTypeNumber);

    return createdTile;
}

// Method for merging Tiles
Tile* Board :: mergeTiles(int type)
{
    int typeNumber = 0;
    
    // Searches for the given type in allTypes vector
    for (int i = 0; i <= (allTypes.size() - 1); i++)
    {
        // New typeNumber must the next one (it is increased by 2, as vector position starts from 0)
        if (allTypes[i] == type) typeNumber = i + 2;
    }

    Tile* createdTile = createTile(typeNumber);

    // If usedTypes has not it maximal predefined leght, the usedTypes vector is updated
    if (usedTypes.size() != Tile6144().getIDNumber()) updateUsedTypes(typeNumber);

    return createdTile;
}

// Method for creating a new Tile instance, according to the given typeNumber
Tile* Board :: createTile(int typeNumber)
{
    switch (typeNumber)
    {
        case 1:
            return new Tile1();
            break;

        case 2:
            return new Tile2();
            break;

        case 3:
            return new Tile3();
            break;

        case 4:
            return new Tile6();
            break;

        case 5:
            return new Tile12();
            break;

        case 6:
            return new Tile24();
            break;

        case 7:
            return new Tile48();
            break;

        case 8:
            return new Tile96();
            break;

        case 9:
            return new Tile192();
            break;

        case 10:
            return new Tile384();
            break;

        case 11:
            return new Tile768();
            break;

        case 12:
            return new Tile1536();
            break;

        case 13:
            return new Tile3072();
            break;

        case 14:
            return new Tile6144();
            break;

        default:
            return nullptr;
            break;
    }
}

// Method for updating usedTypes vector, with Tiles id number corresponding to the typeNumber, if the vector is shorter
void Board :: updateUsedTypes(int typeNumber)
{
    switch (typeNumber)
    {
        case 4:
            if (usedTypes.size() == 3) usedTypes.push_back(Tile6().getIDNumber());
            break;

        case 5:
            if (usedTypes.size() == 4) usedTypes.push_back(Tile12().getIDNumber());
            break;

        case 6:
            if (usedTypes.size() == 5) usedTypes.push_back(Tile24().getIDNumber());
            break;

        case 7:
            if (usedTypes.size() == 6) usedTypes.push_back(Tile48().getIDNumber());
            break;

        case 8:
            if (usedTypes.size() == 7) usedTypes.push_back(Tile96().getIDNumber());
            break;

        case 9:
            if (usedTypes.size() == 8) usedTypes.push_back(Tile192().getIDNumber());
            break;

        case 10:
            if (usedTypes.size() == 9) usedTypes.push_back(Tile384().getIDNumber());
            break;

        case 11:
            if (usedTypes.size() == 10) usedTypes.push_back(Tile768().getIDNumber());
            break;

        case 12:
            if (usedTypes.size() == 11) usedTypes.push_back(Tile1536().getIDNumber());
            break;

        case 13:
            if (usedTypes.size() == 12) usedTypes.push_back(Tile3072().getIDNumber());
            break;

        case 14:
            if (usedTypes.size() == 13) usedTypes.push_back(Tile6144().getIDNumber());
            break;

        default:
            break;
    }
}

// Default constructor, which passes defined values specifying the board properties and initializing containers
Board :: Board() : height(VERTICAL_BOARD_SIZE), width(HORIZONTAL_BOARD_SIZE), size(GRID_SIZE), border(BOARDER_SIZE), gridIsFull(false), insideColour(INSIDE_COLOUR), outsideColour(OUTSIDE_COLOUR)
{
    
    grid = initializeGrid();
    
    allTypes = initializeAllTypes();
    
    usedTypes = initializeUsedTypes();
}

// Default destructor. It deletes all pointers from grid variable
Board :: ~Board()
{
    for (int i = 0; i <= (height - 1); i++)
    {
        for (int j = 0; j <= (width - 1); j++)
        {
            delete grid[i][j];
        }
    }
}

// Method for displaying the grid. It iterates thru all pointers in grid
void Board :: displayGrid(SFMLGraphics* graphic)
{
    int y = 0;
    
    for(int i = 0; i <= (height - 1); i++)
    {
        int x = 0;
        
        for(int j = 0; j <= (width - 1); j++)
        {
            // Calling graphicsDrawSquare method from SFMLGraphics, with values stored in class variables
            graphic->graphicsDrawSquare(size, insideColour, border, outsideColour, (x + border), (y + border));
            
            // Increase of x coordinate by both size of grid and its border
            x += size + border;
        }
        
        // Increase of y coordinate by both size of grid and its border
        y += size + border;
    }
}

// Method for displaying the Tiles. It iterates thru all pointers in grid
void Board :: displayTiles(SFMLGraphics* graphic)
{
    int y = 0;
    
    for(int i = 0; i <= (height - 1); i++)
    {
        int x = 0;
        
        for(int j = 0; j <= (width - 1); j++)
        {
            // If the pointer is different than nullptr it call graphicsDrawSprite method from SFMLGraphics
            if (grid[i][j] != nullptr) graphic->graphicsDrawSprite((x + border), (y + border), grid[i][j]->getIDNumber());
            
            // Increase of x coordinate by both size of grid and its border
            x += size + border;
        }
        
        // Increase of y coordinate by both size of grid and its border
        y += size + border;
    }
}

// Method for moving the Tiles up
void Board :: moveUp()
{
    // Moving each pointer in grid up
    moveTilesUp();
    
    // Searching for empty boxes in last row
    std::vector<int> emptyBoxes = searchForFreeBoxInLastRow();
    
    // If there are no empty boxes the grid is full
    if (emptyBoxes.empty()) gridIsFull = true;
    
    // If not
    if(!gridIsFull)
    {
        int emptyColumnsNumber = int(emptyBoxes.size());
    
        srand(int(time(NULL)));
    
        // It draws a random number from interval <0; emptyColumnsNumber-1>
        int randomColumn = rand() % emptyColumnsNumber;
    
        // It adds a Tiles in lest row and randomly drawn free column
        grid[height - 1][emptyBoxes[randomColumn]] = addTile();
    }
}

// Method for moving Tiles down. It works the former one, with three differences
void Board :: moveDown()
{
    // Moving each pointer in grid down
    moveTilesDown();
    
    // It searches for free boxes in first row
    std::vector<int> emptyBoxes = searchForFreeBoxInFirstRow();
    
    if (emptyBoxes.empty()) gridIsFull = true;
    
    if(!gridIsFull)
    {
        int emptyColumnsNumber = int(emptyBoxes.size());
    
        srand(int(time(NULL)));
    
        int randomColumn = rand() % emptyColumnsNumber;
    
        // It adds a Tiles in first row and random free column
        grid[0][emptyBoxes[randomColumn]] = addTile();
    }
}

// Method for moving Tiles right. It works the former ones, with three differences
void Board :: moveRight()
{
    // Moving each pointer in grid right
    moveTilesRight();
    
    // It searches for free boxes in first column
    std::vector<int> emptyBoxes = searchForFreeBoxInFirstColumn();
    
    if (emptyBoxes.empty()) gridIsFull = true;
    
    if(!gridIsFull)
    {
        int emptyRowsNumber = int(emptyBoxes.size());
    
        srand(int(time(NULL)));
    
        int randomRow = rand() % emptyRowsNumber;
    
        // It adds a Tiles in random free row and first column
        grid[emptyBoxes[randomRow]][0] = addTile();
    }
}

// Method for moving Tiles left. It works the former ones, with three differences
void Board :: moveLeft()
{
    // Moving each pointer in grid left
    moveTilesLeft();
    
    // It searches for free boxes in last column
    std::vector<int> emptyBoxes = searchForFreeBoxInLastColumn();
    
    if (emptyBoxes.empty()) gridIsFull = true;
    
    if(!gridIsFull)
    {
        int emptyRowsNumber = int(emptyBoxes.size());
    
        srand(int(time(NULL)));
    
        int randomRow = rand() % emptyRowsNumber;
    
        // It adds a Tiles in random free row and last column
        grid[emptyBoxes[randomRow]][width - 1] = addTile();
    }
}

// Method, which return the gridIsFull variable
bool Board :: isGridFull()
{
    return gridIsFull;
}

// Method, which iterates thru the entire grid and for not nullptrs calls method for getting Tile points and sums it up to the score variable
int Board :: calculatePoints()
{
    int points = 0;
    
    for (int i = 0; i <= (height - 1); i++)
    {
        for (int j = 0; j <= (width - 1); j++)
        {
            if (grid[i][j] != nullptr) points += grid[i][j]->getPoints();
        }
    }
    
    return points;
}
