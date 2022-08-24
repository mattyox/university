#ifndef Tile_h
#define Tile_h

// Base class used for specifing (by inheritance) further classes (Tiles) according to the game rules
class Tile
{
protected:
    // Variable for storing the identification number connected with Tile
    int idNumber;
    // Variable for storing the value of Tile
    int value;
    
public:
    Tile(int i, int v);
    int getIDNumber();
    int getPoints();
};

#endif /* Tile_h */
