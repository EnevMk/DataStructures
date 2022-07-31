

#ifndef CMD_LINE
#define CMD_LINE

#include "vector.hpp"

#include <exception>
#include <stdexcept>

#include <string>
class CommandLine {

    Vector<std::string> args;

    

    static inline std::string clearWhitespaces(const std::string& str);

public:
    void parseCommand(const std::string& cmd);
    CommandLine();
    CommandLine(const std::string& cmd);
    const std::string& operator[](size_t index) const;
    int count() const ;
};

#endif
