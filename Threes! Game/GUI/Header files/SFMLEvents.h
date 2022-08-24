#ifndef SFMLEvents_h
#define SFMLEvents_h

#include <SFML/Graphics.hpp>

// Class for handling events like key pressing or window closing
class SFMLEvents
{
private:
    // Variable for storing state of arrow up key (true if pressed)
    bool upKeyPressed;
    // Variable for storing state of arrow down key (true if pressed)
    bool downKeyPressed;
    // Variable for storing state of arrow right key (true if pressed)
    bool rightKeyPressed;
    // Variable for storing state of arrow left key (true if pressed)
    bool leftKeyPressed;
    // Variable for storing state of window (true if closed)
    bool windowClosed;
    
public:
    SFMLEvents();
    void setUpKeyPressed(bool state);
    void setDownKeyPressed(bool state);
    void setRightKeyPressed(bool state);
    void setLeftKeyPressed(bool state);
    void setWindowClosed(bool state);
    bool getUpKeyPressed();
    bool getDownKeyPressed();
    bool getRightKeyPressed();
    bool getLeftKeyPressed();
    bool getWindowClosed();
};

#endif /* SFMLEvents_h */
