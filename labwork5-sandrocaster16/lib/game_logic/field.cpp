#include "field.hpp"

bool Field::foundPoint(uint64_t x, uint64_t y){
    for(const Point& point : points){
        if(point.X == x && point.Y == y){
            return true;
        }
    }

    return false;
}

void Field::addPoint(uint64_t x, uint64_t y){
    if(foundPoint(x, y)) return;

    points.push_back({x, y});
}

void Field::addShip(const Ship& ship){
    ships.push_back(ship);
    for(int i=0; i<ship.size_ship; ++i){
        if(ship.direction == 0){
            points.push_back({ship.left, ship.top+i});
        }
        else{
            points.push_back({ship.left+i, ship.top});
        }
    }
}

std::string Field::isPoint(uint64_t x, uint64_t y){
    for(int i=0; i<points.size(); ++i){
        if(points[i].X == x && points[i].Y == y){
            if(i!=points.size()-1 && (points[i+1].X-points[i].X == 0 || points[i+1].Y-points[i].Y == 0)){
                points.erase(points.begin() + i);
                return "hit";
            }
            if(i!=0 && (points[i-1].X-points[i].X == 0 || points[i-1].Y-points[i].Y == 0)){
                points.erase(points.begin() + i);
                return "hit";
            }

            points.erase(points.begin() + i);
            return "kill";
        }
    }

    return "miss";
}

bool Field::isPosition(const Ship& ship){
    for(int i=0; i<ship.size_ship; ++i){
        if(ship.direction == 0){

            for(int x=-1; x<2; ++x){
                for(int y=-1; y<2; ++y){
                    if(foundPoint(ship.left+x, ship.top+i+y)) return false;
                }
            }

        }
        else{

            for(int x=-1; x<2; ++x){
                for(int y=-1; y<2; ++y){
                    if(foundPoint(ship.left+i+x, ship.top+y)) return false;
                }
            }

        }
    }

    return true;
}

void Field::print(){
    std::cout<<"[ships]\n";
    for(const Ship& q : ships){
        std::cout   <<"size: "<<q.size_ship<<'\n'
                    <<"direction: "<<(q.direction==0 ? "vertical\n" : "horizontal\n")
                    <<"left: "<<q.left<<'\n'
                    <<"top: "<<q.top<<"\n\n";
    }

    std::cout<<"[points]\n";
    for(const Point& q : points){
        std::cout<<"x: "<<q.X<<" y: "<<q.Y<<'\n';
    }
}