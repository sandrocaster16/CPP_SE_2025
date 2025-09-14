#pragma once

#include <iostream>
#include <vector>
#include "arguments.hpp"
#include "commands.hpp"
#include "logic_commands.hpp"
#include "../console/console.hpp"

class Parser {
private:
    Console* console_;
    LogicCommands* LogicCommands_;
    Arguments* argument;

public:
    Parser(Console& console, LogicCommands& LogicCommands);
    ~Parser();

    bool isNumber(const std::string& str);
    std::vector<std::string> cut_line(const std::string& line);
    void parse(const std::string& line);
};