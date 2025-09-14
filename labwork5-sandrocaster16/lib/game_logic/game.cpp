#include "game.hpp"

Game::Game(Console& console, Settings& settings, FileWorker& file_worker)
    : field_computer_(new Field)
    , field_player_(new Field)
    , console_(&console)
    , settings_(&settings)
    , file_worker_(&file_worker)
    , last_x_(-1)
    , last_y_(0)
    , all_ships_(-1)
    , koef(3)
{}

Game::~Game(){
    delete field_computer_;
    delete field_player_;
}

void Game::startGame(){
    if(settings_->is_running_){
        console_->SendError("the game is already running");
        return;
    }

    if(
        settings_->size_x_ != 0 &&
        settings_->size_y_ != 0 &&
        settings_->count_ships_[0] != 0 &&
        settings_->count_ships_[1] != 0 &&
        settings_->count_ships_[2] != 0 &&
        settings_->count_ships_[3] != 0 &&

        settings_->size_x_*settings_->size_y_ > koef*(settings_->count_ships_[0]+settings_->count_ships_[1]*2+settings_->count_ships_[2]*3+settings_->count_ships_[3]*4)
    ){
        settings_->is_running_ = true;
        settings_->win_ = -1;
        all_ships_ = (settings_->count_ships_[0]+settings_->count_ships_[1]*2+settings_->count_ships_[2]*3+settings_->count_ships_[3]*4);
        // std::cout<<"all ships: "<<all_ships_<<'\n';

        delete field_player_;
        field_player_ = new Field;
        
        if(!settings_->is_load){
            delete field_computer_;
            field_computer_ = new Field;
            randomShip();
        }

        console_->SendConsole("ok");
    }
    else{
        console_->SendError("requirements have not been met");
    }
}

void Game::stopGame(){
    settings_->is_running_ = false;
    settings_->is_load = false;
    console_->SendConsole("ok");
}

void Game::gameResult(){
    if(field_computer_->points.size() == 0){
        settings_->win_ = 0;
        settings_->is_running_ = false;
    }

    if(field_player_->points.size() == all_ships_){
        settings_->win_ = 1;
        settings_->is_running_ = false;
    }
}

void Game::randomShip(){
    uint64_t* ships = new uint64_t[4]{settings_->count_ships_[0], settings_->count_ships_[1], settings_->count_ships_[2], settings_->count_ships_[3]};

    int direction; // 0 vertical | 1 horizontal
    uint64_t left;
    uint64_t top;

    for(int index=3; index>=0; --index){
        while(ships[index] != 0){
            direction = std::rand() % 2;

            if(direction == 0){
                left = std::rand() % settings_->size_x_;
                top = std::rand() % (settings_->size_y_-index);
            }
            else{
                left = std::rand() % (settings_->size_x_-index);
                top = std::rand() % settings_->size_y_;
            }

            if(field_computer_->isPosition({index+1, direction, left, top})){
                field_computer_->addShip({index+1, direction, left, top});
                --ships[index];
            }
        }
    }

    delete[] ships;

    // field_computer_->print();
}

// shot
void Game::shotIn(uint64_t X, uint64_t Y){
    if(settings_->is_running_){
        console_->SendConsole(field_computer_->isPoint(X, Y));
        gameResult();
    }
    else{
        console_->SendConsole("miss");
    }
}

void Game::shotOut(){
    if(settings_->is_running_){
        if(settings_->strategy_ == 0){
            ++last_x_;
            if(last_x_ == settings_->size_x_){
                ++last_y_;
                last_x_ = 0;
            }
            if(last_y_ == settings_->size_y_){
                last_y_ = 0;
            }
        }
        else{
            last_x_ = std::rand() % settings_->size_x_;
            last_y_ = std::rand() % settings_->size_y_;
        }
        console_->SendConsole(std::to_string(last_x_)+" "+std::to_string(last_y_));
    }
    else{
        console_->SendConsole("0 0");
    }
}

void Game::setResult(std::string result){
    if((result=="hit" || result=="kill") && settings_->is_running_){
        field_player_->addPoint(last_x_, last_y_);
        gameResult();
    }

    console_->SendConsole("ok");
}

void Game::dumpFile(std::string file_name){
    file_worker_->dumpFile(file_name, settings_, field_computer_);
    console_->SendConsole("ok");
}

void Game::loadFile(std::string file_name){
    if(!settings_->is_running_){
        if(file_worker_->loadFile(file_name, settings_, field_computer_)){
            console_->SendConsole("ok");
            settings_->is_load = true;
        }
        else{
            console_->SendError("load file is not open :(");
        }
    }
    else{
        console_->SendError("the game is running now");
    }
}