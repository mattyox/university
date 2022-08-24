#include "SFMLEvents.h"

// Default constructor initializing all member variables are false
SFMLEvents :: SFMLEvents() : upKeyPressed(false), downKeyPressed(false), rightKeyPressed(false), leftKeyPressed(false), windowClosed(false)
{
    ;
}

// Method for setting the arrow up key state
void SFMLEvents :: setUpKeyPressed(bool state)
{
    upKeyPressed = state;
}

// Method for setting the arrow down key state
void SFMLEvents :: setDownKeyPressed(bool state)
{
    downKeyPressed = state;
}

// Method for setting the arrow right key state
void SFMLEvents :: setRightKeyPressed(bool state)
{
    rightKeyPressed = state;
}

// Method for setting the arrow left key state
void SFMLEvents :: setLeftKeyPressed(bool state)
{
    leftKeyPressed = state;
}

// Method for setting the window state
void SFMLEvents :: setWindowClosed(bool state)
{
    windowClosed = state;
}

// Method for returning the arrow up state
bool SFMLEvents :: getUpKeyPressed()
{
    return upKeyPressed;
}

// Method for returning the arrow down state
bool SFMLEvents :: getDownKeyPressed()
{
    return downKeyPressed;
}

// Method for returning the arrow right state
bool SFMLEvents :: getRightKeyPressed()
{
    return rightKeyPressed;
}

// Method for returning the arrow left state
bool SFMLEvents :: getLeftKeyPressed()
{
    return leftKeyPressed;
}

// Method for returning the window state
bool SFMLEvents :: getWindowClosed()
{
    return windowClosed;
}
