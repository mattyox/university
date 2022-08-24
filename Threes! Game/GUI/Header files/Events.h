//
//  Events.h
//  Project_GUI
//
//  Created by Mateusz Wolowczyk on 15/01/2022.
//

#ifndef Events_h
#define Events_h

class Events
{
public:
    virtual void setUpKeyPressed(bool state) = 0;
    virtual void setDownKeyPressed(bool state) = 0;
    virtual void setRightKeyPressed(bool state) = 0;
    virtual void setLeftKeyPressed(bool state) = 0;
    virtual void setWindowClosed(bool state) = 0;
    virtual bool getUpKeyPressed() = 0;
    virtual bool getDownKeyPressed() = 0;
    virtual bool getRightKeyPressed() = 0;
    virtual bool getLeftKeyPressed() = 0;
    virtual bool getWindowClosed() = 0;
    virtual ~Events();
};

#endif /* Events_h */
