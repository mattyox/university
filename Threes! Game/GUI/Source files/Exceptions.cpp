#include "Exceptions.h"

// Constructor of Exceptions class, which assigns value fileName to fileName variable
Exceptions :: Exceptions(const char* fN) : fileName(fN)
{
    ;
}

// Overridden what() method from std::exception
const char* Exceptions :: what() const noexcept
{
    // It prepares a message, which file cannot be opened
    static std::string message = "Could not open file from directory: ";
    
    message += fileName;
    message += "\n";
    
    // Conversion from std::string to const char*
    return message.c_str();
}
