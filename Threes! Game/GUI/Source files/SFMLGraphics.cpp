#include "SFMLGraphics.h"

// Default constructor, which loads all needed data
SFMLGraphics :: SFMLGraphics()
{
    loadColours();

    loadFont();
    
    loadTextures();
}

// Method, which maps colours from SFML definitions to simple std::strings
void SFMLGraphics :: loadColours()
{
    colours["red"] = sf::Color::Red;
    colours["green"] = sf::Color::Green;
    colours["blue"] = sf::Color::Blue;
    colours["cyan"] = sf::Color::Cyan;
    colours["magenta"] = sf::Color::Magenta;
    colours["yellow"] = sf::Color::Yellow;
    colours["black"] = sf::Color::Black;
    colours["white"] = sf::Color::White;
}

// Method for loading font from FONT file
void SFMLGraphics :: loadFont()
{
    // If the operation is unsuccesful, a expection is thrown
    try
    {
        if (!font.loadFromFile(resourcePath() + FONT))
        {
            throw Exceptions(FONT);
        }
    }
    catch (Exceptions &e)
    {
        // Appropriate message is printed to standart error stream and program finishes
        std::cerr << e.what();
        exit(0);
    }
}

// Method for loading textures based on TEXTURE_DIRECTORIES
void SFMLGraphics :: loadTextures()
{
    std::ifstream textureDirectoriesFile(resourcePath() + TEXTURE_DIRECTORIES);
    
    // Firstly it opens the TEXTURE_DIRECTORIES files
    try
    {
        if (textureDirectoriesFile.is_open())
        {
            std::string fileLine;
            
            // Getting consecutive lines (texture paths) from opened textureDirectoriesFile
            while (std::getline(textureDirectoriesFile, fileLine))
            {
                // Creating a SFML texture variable
                sf::Texture texture;
                
                // Loading texture from obatined fileLine directory
                if (texture.loadFromFile(resourcePath() + fileLine))
                {
                    // Pushes back the loaded texture into the vector
                    textures.push_back(texture);
                }
                else
                {
                    // If the operation is unsuccesful, a expection is thrown
                    throw Exceptions(fileLine.c_str());
                }
            }
            
            textureDirectoriesFile.close();
        }
        else
        {
            // If the operation is unsuccesful, a expection is thrown
            throw Exceptions(TEXTURE_DIRECTORIES);
        }
    }
    catch (Exceptions &e)
    {
        // Appropriate message is printed to standart error stream and program finishes
        std::cerr << e.what();
        exit(0);
    }
}

// Method for initializing a SFML RenderWindow instance
void SFMLGraphics :: graphicsInitialize(int width, int height, std::string windowTitle)
{
    window = new sf::RenderWindow(sf::VideoMode(width, height), windowTitle);
}

// Method callign SFML isOpen() method
bool SFMLGraphics :: graphicsWindowIsOpen()
{
    return window->isOpen();
}

// Method for processing events
bool SFMLGraphics :: eventsProcessEvent(SFMLEvents &event)
{
    {
        // Creating SFML event variable
        sf::Event sfmlEvent;
        
        if(window->pollEvent(sfmlEvent))
        {
            event.setUpKeyPressed(sfmlEvent.type == sf::Event::KeyPressed && sfmlEvent.key.code == sf::Keyboard::Up);
            
            event.setDownKeyPressed(sfmlEvent.type == sf::Event::KeyPressed && sfmlEvent.key.code == sf::Keyboard::Down);
            
            event.setRightKeyPressed(sfmlEvent.type == sf::Event::KeyPressed && sfmlEvent.key.code == sf::Keyboard::Right);
            
            event.setLeftKeyPressed(sfmlEvent.type == sf::Event::KeyPressed && sfmlEvent.key.code == sf::Keyboard::Left);
            
            event.setWindowClosed(sfmlEvent.type == sf::Event::Closed);
            
            return true;
        }
        else
        {
            return false;
        }
    }
}

// Method calling the SFML close() method
void SFMLGraphics :: graphicsClose()
{
    window->close();
}

// Method calling the SFML clear() method
void SFMLGraphics :: graphicsClear()
{
    window->clear();
}

// Method for drawing a square
void SFMLGraphics :: graphicsDrawSquare(int side, std::string insideColour, double outsideThickness, std::string outsideColour, int xPosition, int yPosition)
{
    // Creating SFML square variable
    sf::RectangleShape square(sf::Vector2f(side, side));
    
    // Formatting the square
    square.setFillColor(colours[insideColour]);
    square.setOutlineThickness(outsideThickness);
    square.setOutlineColor(colours[outsideColour]);
    square.setPosition(sf::Vector2f(xPosition, yPosition));
    
    // Drawing the square
    window->draw(square);
}

// Method for drawing a sprite
void SFMLGraphics :: graphicsDrawSprite(int xPosition, int yPosition, int textureIndex)
{
    // Creating SFML sprite variable
    sf::Sprite sprite;
    
    // Setting texture of the sprite
    sprite.setTexture(textures.at(textureIndex - 1));
    
    // Setting positon of the sprite
    sprite.setPosition(sf::Vector2f(xPosition, yPosition));
    
    // Drawing the sprite
    window->draw(sprite);
}

// Method for drawing a text string
void SFMLGraphics :: graphicsDrawText(std::string text, int fontSize, std::string colour, int xPosition, int yPosition)
{
    // Creating SFML text variable
    sf::Text tx;
    
    // Formatting the text
    tx.setString(text);
    tx.setFont(font);
    tx.setCharacterSize(fontSize);
    tx.setFillColor(colours[colour]);
    
    // Setting the text position
    sf::FloatRect textRect = tx.getLocalBounds();
    tx.setOrigin((textRect.left + (textRect.width / 2.0f)), (textRect.top  + (textRect.height / 2.0f)));
    tx.setPosition(sf::Vector2f(xPosition, yPosition));
    
    // Drawing the text
    window->draw(tx);
}

// Method calling the SFML display() method
void SFMLGraphics :: graphicsDisplay()
{
    window->display();
}

// Destructor deleting the window instance
SFMLGraphics :: ~SFMLGraphics()
{
    delete window;
}
