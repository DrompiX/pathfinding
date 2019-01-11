#include "Agents.hpp"

string Agent::getName() { return name; }

void Agent::setPosition(unsigned x, unsigned y) {
    position.x = x;
    position.y = y;
}

void Agent::do_action() {}

//---------------------------------------------//

RRH::RRH(unsigned x, unsigned y) {
    name = "R";
    position.x = x;
    position.y = y;
    berries = 6;
    map_data.wolf.x = -1;
    map_data.wolf.y = -1;
    map_data.bear.x = -1;
    map_data.bear.y = -1;
    map_data.granny.x = -1;
    map_data.granny.y = -1;
    map_data.woodcutter.first.x = -1;
    map_data.woodcutter.first.y = -1;
    map_data.woodcutter.second.x = -1;
    map_data.woodcutter.second.y = -1;
    map_data.wcVisited = false;
}

int RRH::getBerries() { return berries; }

void RRH::setBerries(unsigned new_amount) {
    berries = new_amount;
}

void RRH::do_action() {}

//---------------------------------------------//

Granny::Granny(unsigned x, unsigned y) {
    name = "G";
    position.x = x;
    position.y = y;
}

void Granny::do_action() {}
    
//---------------------------------------------//

Wolf::Wolf(unsigned x, unsigned y) {
    name = "W";
    position.x = x;
    position.y = y;
}

void Wolf::do_action() {}

//---------------------------------------------//

Bear::Bear(unsigned x, unsigned y) {
    name = "B";
    position.x = x;
    position.y = y;
}

void Bear::do_action() {}

//---------------------------------------------//

WoodCutter::WoodCutter(unsigned hx, unsigned hy, 
            unsigned wx, unsigned wy, bool h) {
    name = "C";
    point house;
    house.x = hx;
    house.y = hy;

    point work;
    work.x = wx;
    work.y = wy;

    locations.first = house;
    locations.second = work;
    if (h) {
        position.x = hx;
        position.y = hy;
    } else {
        position.x = wx;
        position.y = wy;
    }
}

void WoodCutter::do_action() {}
