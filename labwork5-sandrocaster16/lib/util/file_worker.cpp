#include "file_worker.hpp"

void FileWorker::dumpFile(std::string file_name, Settings* settings_, Field* field_){
    std::ofstream file(file_name);

    file << settings_->size_x_<<' '<<settings_->size_y_<<'\n';
    for(int i=0; i<field_->ships.size(); ++i){
        file <<   field_->ships[i].size_ship<<' '
                <<(field_->ships[i].direction==0 ? 'v' : 'h')<<' '
                <<field_->ships[i].left     <<' '
                <<field_->ships[i].top      <<'\n';
    }

    file.close();
}

bool FileWorker::loadFile(std::string file_name, Settings* settings_, Field* field_){
    std::ifstream file(file_name);

    if(!file.is_open()){
        return false;
    }

    field_->ships.clear();
    field_->points.clear();
    delete[] settings_->count_ships_;
    settings_->count_ships_ = new uint64_t[4]{0,0,0,0};

    file >> settings_->size_x_ >> settings_->size_y_;

    int size_ship, int_direction;
    char direction;
    uint64_t x, y;
    while(file >> size_ship >> direction >> x >> y){
        int_direction = (direction=='v' ? 0 : 1);
        field_->addShip({size_ship, int_direction, x, y});
        ++settings_->count_ships_[size_ship-1];
    }

    file.close();
    return true;
}