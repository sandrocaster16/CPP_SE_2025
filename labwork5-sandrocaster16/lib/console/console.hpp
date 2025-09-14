#pragma once

#include <iostream>

class Console{
private:
    std::string line;

public:
    std::string GetConsole();
    void SendConsole(std::string message);
    void SendConsole(uint64_t message);
    void SendError(std::string error_message);
};