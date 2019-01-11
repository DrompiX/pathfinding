#pragma once

#include "Environment.hpp"
#include "Agents.hpp"

class GameLauncher {
    RRH *hero;
    Environment map;
    vector< pair<int, int> > ans;
    int numberOfSteps;
    unsigned start_time;
    unsigned end_time;
public:
    GameLauncher();
    void playWithBack();
    void playWithAstar();
    void printMap(Environment);
    void printMap();
    int getNumberOfSteps() { return numberOfSteps; }
    unsigned getTime() { return end_time - start_time; }
private:
    pair <int, int>* nextMove(int, int, Environment& a, bool);
    bool checkBounds(int, int);
};
