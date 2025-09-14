#pragma once

#include <iostream>

class Settings{
public:
    // running
    bool is_running_;
    int win_; // -1 not finished | 0 player | 1 computer

    // режим [master, slave]
    int mode_; // 0      1

    // поле
    uint64_t size_x_;
    uint64_t size_y_;

    // колличество кораблей определенного типа
    uint64_t* count_ships_;

    // стратегия [ordered,custom]
    int strategy_; // 0      1

    bool is_load;

    Settings();
    ~Settings();
};