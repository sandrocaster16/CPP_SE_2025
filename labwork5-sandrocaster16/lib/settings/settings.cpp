#include "settings.hpp"

Settings::Settings()
    : is_running_(false)
    , win_(-1)
    , mode_(1)
    , size_x_(0)
    , size_y_(0)
    , count_ships_(new uint64_t[4]{0,0,0,0})
    , strategy_(1)
    , is_load(false)
{}

Settings::~Settings(){
    delete[] count_ships_;
}