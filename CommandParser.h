#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "GameTypes.h"
#include <string>

class CommandParser {
public:
    Command parse(const std::string& input) const;

private:
    static std::string trim(const std::string& text);
    static std::string lowerAscii(std::string text);
};

#endif
