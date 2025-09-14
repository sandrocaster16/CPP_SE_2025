#pragma once

#include <iostream>
#include "field.hpp"
#include "../console/console.hpp"
#include "../settings/settings.hpp"
#include "../util/file_worker.hpp"

class Game{
private:
    Field* field_computer_;
    Field* field_player_;

    Console* console_;
    Settings* settings_;
    FileWorker* file_worker_;

    uint64_t last_x_;
    uint64_t last_y_;
    uint64_t all_ships_;
    int koef;

public:
    Game(Console& console, Settings& settings, FileWorker& file_worker);
    ~Game();

    void startGame();
    void stopGame();

    void gameResult();
    void randomShip();

    // shot
    void shotIn(uint64_t X, uint64_t Y);
    void shotOut();
    void setResult(std::string result);

    void dumpFile(std::string file_name);
    void loadFile(std::string file_name);
};
