//
//  Board.cpp
//  Project
//
//  Created by Mateusz Wolowczyk on 01/01/2022.
//

#include "Board.h"

std::vector<std::vector<Tile*>> Board :: initializeGrid()
{
    std::vector<std::vector<Tile*>> initialGrid;

    for (int i = 1; i <= height; i++)
    {
        initialGrid.push_back(std::vector<Tile*>(width, nullptr));
    }

    return initialGrid;
}

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

std::vector<int> Board :: initializeUsedTypes()
{
    std::vector<int> initialUsedTypes;
    
    initialUsedTypes.push_back(Tile1().getIDNumber());
    initialUsedTypes.push_back(Tile2().getIDNumber());
    
    return initialUsedTypes;
}

void Board :: moveTilesUp()
{
    for (int i = 1; i <= (height - 1); i++)
    {
        for (int j = 0; j <= (width - 1); j++)
        {
            if (((grid[i - 1][j] == nullptr) && (grid[i][j] == nullptr)))
            {
                ; // Do nothing
            }
            else if ((grid[i - 1][j] == nullptr) && (grid[i][j] != nullptr))
            {
                grid[i - 1][j] = createTile(grid[i][j]->getIDNumber());
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
            else if ((grid[i - 1][j] != nullptr) && (grid[i][j] == nullptr))
            {
                ; // Do nothing
            }
            else if (((grid[i - 1][j]->getIDNumber() == Tile6144().getIDNumber()) && (grid[i][j]->getIDNumber() == Tile6144().getIDNumber())) || ((grid[i - 1][j]->getIDNumber() == Tile1().getIDNumber()) && (grid[i][j]->getIDNumber() == Tile1().getIDNumber())) || ((grid[i - 1][j]->getIDNumber() == Tile2().getIDNumber()) && (grid[i][j]->getIDNumber() == Tile2().getIDNumber())))
            {
                ; // Do nothing
            }
            else if (((grid[i - 1][j]->getIDNumber() == Tile1().getIDNumber()) && ((grid[i][j])->getIDNumber() == Tile2().getIDNumber())) || ((grid[i - 1][j]->getIDNumber() == Tile2().getIDNumber()) && ((grid[i][j])->getIDNumber() == Tile1().getIDNumber())))
            {
                delete grid[i - 1][j];
                grid[i - 1][j] = createTile(Tile3().getIDNumber());
                if (usedTypes.size() == 2) usedTypes.push_back(Tile3().getIDNumber());
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
            else if ((grid[i - 1][j]->getIDNumber()) == ((grid[i][j]->getIDNumber())))
            {
                delete grid[i - 1][j];
                grid[i - 1][j] = mergeTiles(grid[i][j]->getIDNumber());
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
    }
}

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

void Board :: moveTilesDown()
{
    for (int i = (height - 2); i >= 0; i--)
    {
        for (int j = 0; j <= (width - 1); j++)
        {
            if (((grid[i][j] == nullptr) && (grid[i + 1][j] == nullptr)))
            {
                ; // Do nothing
            }
            else if ((grid[i][j] == nullptr) && (grid[i + 1][j] != nullptr))
            {
                ; // Do nothing
            }
            else if ((grid[i][j] != nullptr) && (grid[i + 1][j] == nullptr))
            {
                grid[i + 1][j] = createTile(grid[i][j]->getIDNumber());
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
            else if (((grid[i][j]->getIDNumber() == Tile6144().getIDNumber()) && (grid[i + 1][j]->getIDNumber() == Tile6144().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && (grid[i + 1][j]->getIDNumber() == Tile1().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && (grid[i + 1][j]->getIDNumber() == Tile2().getIDNumber())))
            {
                ; // Do nothing
            }
            else if (((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && ((grid[i + 1][j])->getIDNumber() == Tile2().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && ((grid[i + 1][j])->getIDNumber() == Tile1().getIDNumber())))
            {
                delete grid[i + 1][j];
                grid[i + 1][j] = createTile(Tile3().getIDNumber());
                if (usedTypes.size() == 2) usedTypes.push_back(Tile3().getIDNumber());
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
            else if ((grid[i][j]->getIDNumber()) == ((grid[i + 1][j]->getIDNumber())))
            {
                delete grid[i + 1][j];
                grid[i + 1][j] = mergeTiles(grid[i][j]->getIDNumber());
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
    }
}

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

void Board :: moveTilesRight()
{
    for (int i = 0; i <= (height - 1); i++)
    {
        for (int j = (width - 1); j >= 1; j--)
        {
            if (((grid[i][j - 1] == nullptr) && (grid[i][j] == nullptr)))
            {
                ; // Do nothing
            }
            else if ((grid[i][j - 1] == nullptr) && (grid[i][j] != nullptr))
            {
                ; // Do nothing
            }
            else if ((grid[i][j - 1] != nullptr) && (grid[i][j] == nullptr))
            {
                grid[i][j] = createTile(grid[i][j - 1]->getIDNumber());
                delete grid[i][j - 1];
                grid[i][j - 1] = nullptr;
            }
            else if (((grid[i][j - 1]->getIDNumber() == Tile6144().getIDNumber()) && (grid[i][j]->getIDNumber() == Tile6144().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && (grid[i][j - 1]->getIDNumber() == Tile1().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && (grid[i][j - 1]->getIDNumber() == Tile2().getIDNumber())))
            {
                ; // Do nothing
            }
            else if (((grid[i][j - 1]->getIDNumber() == Tile1().getIDNumber()) && ((grid[i][j])->getIDNumber() == Tile2().getIDNumber())) || ((grid[i][j - 1]->getIDNumber() == Tile2().getIDNumber()) && ((grid[i][j])->getIDNumber() == Tile1().getIDNumber())))
            {
                delete grid[i][j];
                grid[i][j] = createTile(Tile3().getIDNumber());
                if (usedTypes.size() == 2) usedTypes.push_back(Tile3().getIDNumber());
                delete grid[i][j - 1];
                grid[i][j - 1] = nullptr;
            }
            else if ((grid[i][j - 1]->getIDNumber()) == ((grid[i][j]->getIDNumber())))
            {
                delete grid[i][j];
                grid[i][j] = mergeTiles(grid[i][j - 1]->getIDNumber());
                delete grid[i][j - 1];
                grid[i][j - 1] = nullptr;
            }
        }
    }
}

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

void Board :: moveTilesLeft()
{
    for (int i = 0; i <= (height - 1); i++)
    {
        for (int j = 0; j <= (width - 2); j++)
        {
            if (((grid[i][j] == nullptr) && (grid[i][j + 1] == nullptr)))
            {
                ; // Do nothing
            }
            else if ((grid[i][j] == nullptr) && (grid[i][j + 1] != nullptr))
            {
//                std::cout << "Just move right from: " << i << " and " << j + 1 << ", to: " << i << " and " << j << "\n";
                
                grid[i][j] = createTile(grid[i][j + 1]->getIDNumber());
                delete grid[i][j + 1];
                grid[i][j + 1] = nullptr;
            }
            else if ((grid[i][j] != nullptr) && (grid[i][j + 1] == nullptr))
            {
                ; // Do nothing
            }
            else if (((grid[i][j]->getIDNumber() == Tile6144().getIDNumber()) && (grid[i][j + 1]->getIDNumber() == Tile6144().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && (grid[i][j + 1]->getIDNumber() == Tile1().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && (grid[i][j + 1]->getIDNumber() == Tile2().getIDNumber())))
            {
                ; // Do nothing
            }
            else if (((grid[i][j]->getIDNumber() == Tile1().getIDNumber()) && ((grid[i][j + 1])->getIDNumber() == Tile2().getIDNumber())) || ((grid[i][j]->getIDNumber() == Tile2().getIDNumber()) && ((grid[i][j + 1])->getIDNumber() == Tile1().getIDNumber())))
            {
                delete grid[i][j];
                grid[i][j] = createTile(Tile3().getIDNumber());
                if (usedTypes.size() == 2) usedTypes.push_back(Tile3().getIDNumber());
                delete grid[i][j + 1];
                grid[i][j + 1] = nullptr;
            }
            else if ((grid[i][j]->getIDNumber()) == ((grid[i][j + 1]->getIDNumber())))
            {
                delete grid[i][j];
                grid[i][j] = mergeTiles(grid[i][j + 1]->getIDNumber());
                delete grid[i][j + 1];
                grid[i][j + 1] = nullptr;
            }
        }
    }
}

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

Tile* Board :: addTile()
{
    int usedTypesNumber = int(usedTypes.size());
    
    int randomTypeNumber = (rand() % usedTypesNumber) + 1;
    
    Tile* createdTile = createTile(randomTypeNumber);

    return createdTile;
}

Tile* Board :: mergeTiles(int type)
{
    int typeNumber = 0;
    
    for (int i = 0; i <= (allTypes.size() - 1); i++)
    {
        if (allTypes[i] == type) typeNumber = i + 2;
    }

    Tile* createdTile = createTile(typeNumber);

    if (usedTypes.size() != Tile6144().getIDNumber()) updateUsedTypes(typeNumber);

    return createdTile;
}

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

Board :: Board() : height(VERTICAL_BOARD_SIZE), width(HORIZONTAL_BOARD_SIZE)
{
    
    grid = initializeGrid();
    
    allTypes = initializeAllTypes();
    
    usedTypes = initializeUsedTypes();
    
    gridIsFull = false;
}

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

void Board :: displayBoard()
{
    std::cout << "\n";
    
    for (int i = 0; i <= (height - 1); i++)
    {
        for (int j = 0; j <= (width - 1); j++)
        {
            if (grid[i][j] != nullptr)
            {
                grid[i][j]->display();
            }
            else
            {
//                std::cout << "Empty";
                std::cout << "0";
            }
            
//            std::cout << "\t\t";
            std::cout << "\t";
        }
        
//        std::cout << "\n\n";
        std::cout << "\n";
    }
    
    std::cout << "\n";
}

void Board :: moveUp()
{
    moveTilesUp();
    
    std::vector<int> emptyBoxes = searchForFreeBoxInLastRow();
    
    if (emptyBoxes.empty()) gridIsFull = true;
    
    if(!gridIsFull)
    {
        int emptyColumnsNumber = int(emptyBoxes.size());
    
        srand(int(time(NULL)));
    
        int randomColumn = rand() % emptyColumnsNumber;
    
        grid[height - 1][emptyBoxes[randomColumn]] = addTile();
    }
}

void Board :: moveDown()
{
    moveTilesDown();
    
    std::vector<int> emptyBoxes = searchForFreeBoxInFirstRow();
    
    if (emptyBoxes.empty()) gridIsFull = true;
    
    if(!gridIsFull)
    {
        int emptyColumnsNumber = int(emptyBoxes.size());
    
        srand(int(time(NULL)));
    
        int randomColumn = rand() % emptyColumnsNumber;
    
        grid[0][emptyBoxes[randomColumn]] = addTile();
    }
}

void Board :: moveRight()
{
    moveTilesRight();
    
    std::vector<int> emptyBoxes = searchForFreeBoxInFirstColumn();
    
    if (emptyBoxes.empty()) gridIsFull = true;
    
    if(!gridIsFull)
    {
        int emptyRowsNumber = int(emptyBoxes.size());
    
        srand(int(time(NULL)));
    
        int randomRow = rand() % emptyRowsNumber;
    
        grid[emptyBoxes[randomRow]][0] = addTile();
    }
}

void Board :: moveLeft()
{
    moveTilesLeft();
    
    std::vector<int> emptyBoxes = searchForFreeBoxInLastColumn();
    
    if (emptyBoxes.empty()) gridIsFull = true;
    
    if(!gridIsFull)
    {
        int emptyRowsNumber = int(emptyBoxes.size());
    
        srand(int(time(NULL)));
    
        int randomRow = rand() % emptyRowsNumber;
    
        grid[emptyBoxes[randomRow]][width - 1] = addTile();
    }
}

bool Board :: isGridFull()
{
    return gridIsFull;
}

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
