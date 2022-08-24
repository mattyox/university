//
//  Graphics.h
//  Project_GUI
//
//  Created by Mateusz Wolowczyk on 15/01/2022.
//

#ifndef Graphics_h
#define Graphics_h

#include <string>

#include "Events.h"

class Graphics
{
public:
    virtual void graphicsInitialize(int height, int width, std::string windowTitle) = 0;
    virtual bool graphicsWindowIsOpen() = 0;
    virtual Events* eventsCreateEvent() = 0;
    virtual bool eventsProcessEvent(Events& event) = 0;
    virtual void graphicsClose() = 0;
    virtual void graphicsClear() = 0;
    virtual void graphicsPrintSquare(int xPosition, int yPosition, int side, std::string insideColour, std::string outsideColour, double outsideThickness) = 0;
    virtual void graphicsPrintSprite(int xPosition, int yPosition, std::string texturePath) = 0;
    virtual void graphicsPrintText(std::string text, int xPosition, int yPosition, int height, int width, int size, std::string colour) = 0;
    virtual void graphicsDisplay() = 0;
    virtual ~Graphics();
};

#endif /* Graphics_h */
