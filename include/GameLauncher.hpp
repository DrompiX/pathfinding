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
    bool checkBounds(int, int);
    void printMap(Environment);
    void printMap();
    pair <int, int>* nextMove(int, int, Environment& a, bool);
    int getNumberOfSteps() { return numberOfSteps; }
    unsigned getTime() { return end_time - start_time; }
};