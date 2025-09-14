#pragma once

#include <iostream>
#include <vector>

class Field{
private:
    struct Ship{
        int size_ship;
        int direction; // 0 vertical | 1 horizontal
        uint64_t left;
        uint64_t top;
    };

    struct Point{
        uint64_t X;
        uint64_t Y;
    };

public:
    std::vector<Ship> ships;
    std::vector<Point> points;

    bool foundPoint(uint64_t x, uint64_t y);

    void addPoint(uint64_t x, uint64_t y);
    void addShip(const Ship& ship);

    std::string isPoint(uint64_t x, uint64_t y);
    bool isPosition(const Ship& ship);
    
    void print();
};