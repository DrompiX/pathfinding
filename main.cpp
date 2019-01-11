#include <iostream>
#include <vector>
#include <queue>
#include <cstdio>
#include <ctime>
#include "Agents.hpp"
#include "Environment.hpp"
#include "GameLauncher.hpp"

using namespace std;

const int TESTS = 100;

int main(void) {
    srand(time(0));
    freopen("results.txt", "w", stdout); 
    int loses_back = 0, wins_back = 0, steps_back = 0;
    int loses_astar = 0, wins_astar = 0, steps_astar = 0;
    double time_back = 0.0, time_astar = 0.0;
    for (int i = 0; i < TESTS; i++) {
        GameLauncher game;
        game.playWithBack();
        int numOfSteps = game.getNumberOfSteps();
        cout << "BACTRACKING algorithm: " << endl;
        if (numOfSteps == -1) {
            cout << "LOSING MAP!" << endl;
            loses_back++;
        } else {
            cout << "STEPS: " << numOfSteps << endl;
            wins_back++;
            steps_back += numOfSteps;
        }
        cout << "TIME: " << (double)game.getTime()/CLOCKS_PER_SEC * 1000  << "ms" << endl;
        time_back += (double)game.getTime()/CLOCKS_PER_SEC * 1000;
        
        game.playWithAstar();
        cout << endl << "A* algorithm: " << endl;
        numOfSteps = game.getNumberOfSteps();
        if (numOfSteps == -1) {
            cout << "LOSING MAP!" << endl;
            loses_astar++;
        } else {
            cout << "STEPS: " << numOfSteps << endl;
            wins_astar++;
            steps_astar += numOfSteps;
        }
        cout << "TIME: " << (double)game.getTime()/CLOCKS_PER_SEC * 1000  << "ms" << endl;
        time_astar += (double)game.getTime()/CLOCKS_PER_SEC * 1000;
    }
    cout << endl;
    cout << "Statistics for Backtracking algorithm: " << endl;
    cout << "Amount of loses: " << loses_back << endl;
    cout << "Amount of wins: " << wins_back << endl;
    cout << "Failures rate: " << (double)loses_back / TESTS << endl;
    cout << "Wins rate: " << (double)wins_back / TESTS << endl;
    cout << "Average amount of steps: " << (double)steps_back / wins_back << endl;
    cout << "Average time is: " << (double)time_back/(double)TESTS << " ms" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Statistics for A* algorithm: " << endl;
    cout << "Amount of loses: " << loses_astar << endl;
    cout << "Amount of wins: " << wins_astar << endl;
    cout << "Failures rate: " << (double)loses_astar / TESTS << endl;
    cout << "Wins rate: " << (double)wins_astar / TESTS << endl;
    cout << "Average amount of steps: " << (double)steps_astar / wins_astar << endl;
    cout << "Average time is: " << (double)time_astar/(double)TESTS << " ms" << endl;
    return 0;
}
