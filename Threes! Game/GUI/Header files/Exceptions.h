#ifndef Exceptions_h
#define Exceptions_h

#include <exception>
#include <string>

// Class for exceptions derived from std::exception
class Exceptions: public std::exception
{
    // Variable for storing the name of file, which cannot be opened
    const char* fileName;
    
public:
    Exceptions(const char* fN);
    const char* what() const noexcept override;
};

#endif /* Exceptions_h */
