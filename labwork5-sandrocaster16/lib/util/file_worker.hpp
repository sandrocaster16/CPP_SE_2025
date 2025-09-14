#pragma once

#include <iostream>
#include <fstream>
#include "../settings/settings.hpp"
#include "../game_logic/field.hpp"

class FileWorker{
public:
    // files 
    void dumpFile(std::string file_name, Settings* settings_, Field* field_);
    bool loadFile(std::string file_name, Settings* settings_, Field* field_);
};