#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
// от 1 до 100
// std::rand() % 100 + 1
//              range  от


// сборка
#include "commands/commands.hpp"
#include "commands/arguments.hpp"
#include "settings/settings.hpp"
#include "util/file_worker.hpp"
#include "game_logic/field.hpp"
#include "game_logic/game.hpp"
#include "commands/logic_commands.hpp"
#include "commands/parser.hpp"
#include "console/console.hpp"


class BattleShip{
private:
    Settings settings_;
    Console console_;
    Parser parser_;
    FileWorker file_worker_;

    Game game_; 
    LogicCommands LogicCommands_;
    
public:
    BattleShip();
    ~BattleShip() = default;

    void start();
};