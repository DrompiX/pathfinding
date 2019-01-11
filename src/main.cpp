#include <iostream>
#include <vector>
#include <queue>
#include <cstdio>
#include <ctime>
#include "GameLauncher.hpp"

const int TESTS = 100;

using namespace std;

struct Statistics {
    int loses;
    int wins;
    int steps;
    double time;
};

void get_test_report(unique_ptr<GameLauncher>& game, Statistics& report);

void run_test(unique_ptr<GameLauncher> &game,
                    Statistics &back_report,
                    Statistics &astar_report);

void print_full_report(const Statistics& back, const Statistics& astar);

void test_algorithms();

int main(void) {
    srand(time(0));
    freopen("results.txt", "w", stdout); 
    test_algorithms();
    return 0;
}

void test_algorithms() {
    Statistics back_report, astar_report = {0, 0, 0, 0.0};

    for (int i = 0; i < TESTS; i++) {
        unique_ptr<GameLauncher> game(new GameLauncher);
        run_test(game, back_report, astar_report);
    }
    
    print_full_report(back_report, astar_report);
}

void run_test(unique_ptr<GameLauncher> &game,
                    Statistics &back_report,
                    Statistics &astar_report) {
    game->playWithBack();
    cout << "BACTRACKING algorithm: " << endl;
    get_test_report(game, back_report);

    game->playWithAstar();
    cout << endl << "A* algorithm: " << endl;
    get_test_report(game, astar_report);
}

void get_test_report(unique_ptr<GameLauncher>& game, Statistics& report) {
    int numOfSteps = game->getNumberOfSteps();
    if (numOfSteps == -1) {
        cout << "LOSING MAP!" << endl;
        report.loses++;
    } else {
        cout << "STEPS: " << numOfSteps << endl;
        report.wins++;
        report.steps += numOfSteps;
    }
    cout << "TIME: " << (double)game->getTime() / CLOCKS_PER_SEC * 1000  << "ms" << endl;
    report.time += (double)game->getTime() / CLOCKS_PER_SEC * 1000;
}

void print_full_report(const Statistics& back, const Statistics& astar) {
    cout << endl;
    cout << "Statistics for Backtracking algorithm: " << endl;
    cout << "Amount of loses: " << back.loses << endl;
    cout << "Amount of wins: " << back.wins << endl;
    cout << "Failures rate: " << (double)back.loses / TESTS << endl;
    cout << "Wins rate: " << (double)back.wins / TESTS << endl;
    cout << "Average amount of steps: " << (double)back.steps / back.wins << endl;
    cout << "Average time is: " << (double)back.time / (double)TESTS << " ms" << endl;
    cout << "---------------------------------------" << endl;
    cout << "Statistics for A* algorithm: " << endl;
    cout << "Amount of loses: " << astar.loses << endl;
    cout << "Amount of wins: " << astar.wins << endl;
    cout << "Failures rate: " << (double)astar.loses / TESTS << endl;
    cout << "Wins rate: " << (double)astar.wins / TESTS << endl;
    cout << "Average amount of steps: " << (double)astar.steps / astar.wins << endl;
    cout << "Average time is: " << (double)astar.time / (double)TESTS << " ms" << endl;
}
