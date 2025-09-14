#pragma once

#include <iostream>
#include "arguments.hpp"
#include "commands.hpp"
#include "../console/console.hpp"
#include "../settings/settings.hpp"
#include "../game_logic/game.hpp"

class LogicCommands {
private:
    Console* console_;
    Settings* settings_;
    Game* game_;

public:
    LogicCommands(Console& console, Settings& settings, Game& game);

    // code
    void errorCommand();
    void sayPong();
    void exitProgram();
    void createGame(std::string mode);

    // set
    void setHeight(uint64_t height);
    void setWidth(uint64_t width);
    void setCount(uint64_t ship, uint64_t count);
    void setStrategy(std::string q);

    // get
    void getHeight();
    void getWidth();
    void getCount(uint64_t ship);

    // other
    void isFinished();
    void amIWin();
    void amILose();

    // do
    void doCommand(Arguments* arg);
};