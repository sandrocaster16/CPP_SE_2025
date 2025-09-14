#include "BattleShip.hpp"

BattleShip::BattleShip()
    : LogicCommands_(console_, settings_, game_)
    , parser_(console_, LogicCommands_)
    , game_(console_, settings_, file_worker_)
{
    std::srand(std::time(0)); //random
}

void BattleShip::start(){
    while(true){
        parser_.parse(console_.GetConsole());
    }
}