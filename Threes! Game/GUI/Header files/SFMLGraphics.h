#ifndef SFMLGraphics_h
#define SFMLGraphics_h

#include <map>
#include <vector>
#include <fstream>
#include <iostream>

#include "ResourcePath.hpp"
#include "SFMLEvents.h"

#include "Exceptions.h"

#define FONT "SFUIDisplay-Regular.otf"
#define TEXTURE_DIRECTORIES "texturesDirectories.txt"

class SFMLGraphics
{
private:
    // Variable for storing a window instance
    sf::RenderWindow* window;
    // Variable std::map for storing the mapping of colours from SFML definitions to simple std::strings
    std::map<std::string, sf::Color> colours;
    // Variable for storing loaded font
    sf::Font font;
    // Variable std::vector for storing loaded textures
    std::vector<sf::Texture> textures;

public:
    SFMLGraphics();
    void loadColours();
    void loadFont();
    void loadTextures();
    void graphicsInitialize(int height, int width, std::string windowTitle);
    bool graphicsWindowIsOpen();
    bool eventsProcessEvent(SFMLEvents &event);
    void graphicsClose();
    void graphicsClear();
    void graphicsDrawSquare(int side, std::string insideColour, double outsideThickness, std::string outsideColour, int xPosition, int yPosition);
    void graphicsDrawSprite(int xPosition, int yPosition, int textureIndex);
    void graphicsDrawText(std::string text, int fontSize, std::string colour, int xPosition, int yPosition);
    void graphicsDisplay();
    ~SFMLGraphics();
};

#endif /* SFMLGraphics_h */
