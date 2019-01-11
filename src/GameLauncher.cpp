#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include "GameLauncher.hpp"

using namespace std;

const int bear_g = 1000;
const int start_value = 12345;
int dirx[4] = {0, 1, 0, -1};
int diry[4] = {1, 0, -1, 0};
bool checked[SIZE][SIZE];
int endx;
int endy;

// Constructor
GameLauncher::GameLauncher() {
    map.fillEnvironment();
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            checked[i][j] = false;
}

/**
 * This function calculates the best next cell to move,
 * according to distance between RRH and granny
 * curx - current row where RRH
 * cury - current column where RRH
 * hero_map - map of knowledge for RRH
 * risk - is risk mode turned on
 */
pair <int, int>* GameLauncher::nextMove(int curx, int cury, 
        Environment& hero_map, bool risk) {
    int min_path = start_value;
    int id = -1;
    for (int i = 0; i < 4; i++) {
        int cx, cy;
        cx = curx + dirx[i];
        cy = cury + diry[i];
        int distance = abs(cx - endx) + abs(cy - endy);

        if (checkBounds(cx, cy)) {
            if (map.getCell(cx, cy).feel_wolf)
                hero_map.setWolfFeel(cx, cy);
            else if (map.getCell(cx, cy).feel_bear && !risk)
                hero_map.setBearFeel(cx, cy);
            else if (!checked[cx][cy] && distance < min_path) {
                id = i;
                min_path = distance;
            }
        }
    }
    if (min_path != start_value) {
        int nextx = curx + dirx[id];
        int nexty = cury + diry[id];
        WoodCutter* wc;
        wc = dynamic_cast<WoodCutter*>(map.getCell(nextx, nexty).actor);
        if (wc != nullptr) {
            hero_map.setAgent(nextx, nexty, wc);
            hero->map_data.wcVisited = true; 
            hero->setBerries(6);
        }

        if (map.getCell(nextx, nexty).feel_bear)
            hero->setBerries(hero->getBerries() - 2);
        return new pair<int, int>(nextx, nexty);

    }
    return nullptr;
}

/** 
 * Try to solve the problem with Backtracking 
 */
void GameLauncher::playWithBack() {
    Environment hero_map;
    numberOfSteps = 0;
    start_time = clock();
    cout << endl << "*********************" << endl;
    printMap(map);
 
    hero = dynamic_cast<RRH*>(map.getCell(0, 0).actor);
    // Set RRH at (0, 0)
    hero_map.setAgent(0, 0, hero);
    
    // Set destination - Granny.
    endx = hero->map_data.granny.x;
    endy = hero->map_data.granny.y;
    hero_map.setAgent(endx, endy, map.getCell(endx, endy).actor);
    
    // Current position of RRH on the map
    int curx = 0, cury = 0;
    // Path of RRH
    vector < pair<int, int> > path;
    // When there is no opportunity to reach granny
    // without going through bear, it's time to risk
    // and this mode activates in this time.
    bool riskMode = false;
    while (true) {
        // Mark cell as visited
        checked[curx][cury] = true;
        // Add to path
        path.push_back(make_pair(curx, cury));
        // If we found Granny:
        if (curx == endx && cury == endy) {
            if (hero->getBerries() == 6 || 
                    (riskMode && !hero->map_data.wcVisited && hero->getBerries() > 0)) {
                //cout << endl << "THE WINNING PATH WAS FOUND!" << endl;
                while (!path.empty()) {
                    int x = path.back().first;
                    int y = path.back().second;
                    hero_map.setPathCell(x, y);
                    path.pop_back();
                }
                if (!(hero->getBerries() == 6)) {
                    int wchomex, wchomey;
                    wchomex = hero->map_data.woodcutter.first.x;
                    wchomey = hero->map_data.woodcutter.first.y;
                    WoodCutter* wc1 = dynamic_cast<WoodCutter*>(map.getCell(wchomex, wchomey).actor);
                    int wcworkx, wcworky;
                    wcworkx = hero->map_data.woodcutter.second.x;
                    wcworky = hero->map_data.woodcutter.second.y;
                    WoodCutter* wc2 = dynamic_cast<WoodCutter*>(map.getCell(wcworkx, wcworky).actor);
                    // If we reached granny without full basket and hadn't 
                    // visited Woodcutter yet, we will go to wc's location
                    // and back in order to collect full basket of berries
                    if (wc1 != nullptr)
                        numberOfSteps += (abs(endx - wc1->getPosition().x) + abs(endy - wc1->getPosition().y)) * 2;    
                    else
                        numberOfSteps += (abs(endx - wc2->getPosition().x) + abs(endy - wc2->getPosition().y)) * 2;    
                }
                break;
            }
            //cout << "RED RIDING HOOD CAME TO GRANNY" << endl;
            //cout << "WITHOUT FULL BASKET! SHE LOST!" << endl;
            numberOfSteps = -1;
            break;
        }
        // Select next best direction to move
        pair <int, int>* moveTo = nextMove(curx, cury, hero_map, riskMode);
        // If we can't move anywhere - it's time for backtracking
        if (moveTo == nullptr) {
            while (!path.empty()) {
                int fi = path.back().first;
                int se = path.back().second;
                hero_map.setAgent(curx, cury, nullptr);
                hero_map.setAgent(fi, se, hero);
                curx = fi; cury = se;
                moveTo = nextMove(fi, se, hero_map, riskMode);
                if (moveTo == nullptr) {
                    path.pop_back();
                    numberOfSteps++;
                } else
                    break;
            }
        }
        // If RRH reached the start point, either risk mode is 
        // activated if Woodcutter was not visited or 
        // the game ends, because there is no any chance to win
        if (path.empty() || (hero->getBerries() == 0)) {
            if (hero->map_data.wcVisited || riskMode) {
                //cout << "*****************" << endl;
                //cout << "MAP IS UNSOLVABLE" << endl;
                //cout << "*****************" << endl;
                numberOfSteps = -1;
                break;
            } else {
                for (int i = 0; i < SIZE; i++)
                    for (int j = 0; j < SIZE; j++)
                        checked[i][j] = false;
                //cout << endl << "RISK MODE IS ON!!!" << endl;
                riskMode = true;
                continue;
            }
        }
        // Move to selected cell
        numberOfSteps++;
        hero_map.setAgent(curx, cury, nullptr);
        curx = (*moveTo).first;
        cury = (*moveTo).second;
        hero_map.setAgent(curx, cury, hero);
    }
    end_time = clock();
}

/**
 * Struct which represents cell for A* alghorithm
 * h - heuristics
 * g - cost
 * f = h + g
 * prev - cell, from which we came
 */
struct _Cell {
    int x, y;
    int h, g, f;
    _Cell* prev;
    bool operator< (const _Cell& lhs) const {
        if (this->f == lhs.f) {
            if (this->h > lhs.h)
                return true;
            else
                return false;
        } else {
            return this->f > lhs.f ? true : false;
        }
    }
};


/** 
 * Try to solve the problem with A* algorithm 
 */
void GameLauncher::playWithAstar() {
    _Cell hero_map[SIZE][SIZE];
    numberOfSteps = 0;
    start_time = clock();
        
    RRH* hero = dynamic_cast<RRH*>(map.getCell(0, 0).actor);
    // Coordinates of granny
    endx = hero->map_data.granny.x;
    endy = hero->map_data.granny.y;
    
    // Initialize field
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            hero_map[i][j].x = i;
            hero_map[i][j].y = j;
            hero_map[i][j].h = abs(i - endx) + abs(j - endy);
            hero_map[i][j].g = start_value; 
            hero_map[i][j].f = start_value;
            hero_map[i][j].prev = nullptr;
        }
    }

    // Create priority queue to store next best cell to move
    priority_queue<_Cell> toCheck;
    // Array of checked cells
    bool checked[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            checked[i][j] = false;
    
    // Set start default values
    hero_map[0][0].g = 0;
    hero_map[0][0].f = hero_map[0][0].h;
    toCheck.push(hero_map[0][0]);

    while (!toCheck.empty()) {
        _Cell current = toCheck.top();
        toCheck.pop();
        int curx = current.x; int cury = current.y;
        checked[curx][cury] = true;
        if (current.f > bear_g * 3 && current.f != start_value) {
            //cout << "RED RIDING HAVE LOST ALL BERRIES!" << endl;
            //cout << "LOSE MAP" << endl;
            numberOfSteps = -1;
            end_time = clock();
            return;
        }

        // If we found granny: 
        if (curx == endx && cury == endy) {
            _Cell *temp = &hero_map[curx][cury];
            if (temp->f > bear_g && hero->map_data.wcVisited) {
                 //cout << "LOSE MAP" << endl;
                numberOfSteps = -1;
                end_time = clock();
                return;
            }
            int wchomex, wchomey;
            wchomex = hero->map_data.woodcutter.first.x;
            wchomey = hero->map_data.woodcutter.first.y;
            WoodCutter* wc1 = dynamic_cast<WoodCutter*>(map.getCell(wchomex, wchomey).actor);
            int wcworkx, wcworky;
            wcworkx = hero->map_data.woodcutter.second.x;
            wcworky = hero->map_data.woodcutter.second.y;
            WoodCutter* wc2 = dynamic_cast<WoodCutter*>(map.getCell(wcworkx, wcworky).actor);
            // If we reached granny without full basket and hadn't 
            // visited Woodcutter yet, we will go to wc's location
            // and back in order to collect full basket of berries
            if (temp->f > bear_g && !hero->map_data.wcVisited) {
                if (wc1 != nullptr)
                    numberOfSteps = (abs(endx - wc1->getPosition().x) + abs(endy - wc1->getPosition().y)) * 2;    
                else
                    numberOfSteps = (abs(endx - wc2->getPosition().x) + abs(endy - wc2->getPosition().y)) * 2;    
            }

            while(temp != nullptr) {
                ans.push_back(make_pair(temp->x, temp->y));
                temp = temp->prev;
            }
            numberOfSteps += ans.size() - 1;
            end_time = clock();
            return;
        }
        
        // Recalculate all neighbours of the current cell
        for (int i = 0; i < 4; i++) {
            int cx, cy;
            cx = curx + dirx[i];
            cy = cury + diry[i];

            if (checkBounds(cx, cy) && !checked[cx][cy]
                    && !map.getCell(cx, cy).feel_wolf) {
                
                _Cell* ch = &hero_map[cx][cy];
                // If no bear then cost is 1
                if (!map.getCell(cx, cy).feel_bear) {
                    if (ch->g > current.g + 1) {
                        ch->g = current.g + 1;
                        ch->prev = &hero_map[curx][cury];
                    }
                // If there is a bear in the cell (cx, cy)
                // then cost is bear_g = 1000
                } else {
                    if (ch->g > current.g + 1 + bear_g) {
                        ch->g = current.g + 1 + bear_g;
                        ch->prev = &hero_map[curx][cury];
                    }
                }
                WoodCutter* wc;
                wc = dynamic_cast<WoodCutter*>(map.getCell(cx, cy).actor);
                // If we found a Woodcutter he gives us a full basket
                // And change current cost to normal(without bears' influence on score)
                // So if we went through 1-2 cells of bear, the score is 20XX (where
                // X is every digit), and WC makes XX from it.
                if (wc != nullptr) {
                    hero->map_data.wcVisited = true;
                    ch->g = ch->g - ((ch->g) / bear_g) * bear_g;
                    ch->f = ch->h + ch->g;
                }
                ch->f = ch->h + ch->g;
                toCheck.push(hero_map[cx][cy]); 
            }
        }
    }
    numberOfSteps = -1;
    end_time = clock();
}

/**
 * Check bound of the field
 */
bool GameLauncher::checkBounds(int i, int j) {
    if (i < SIZE && i >= 0 && j < SIZE && j >= 0)
        return true;
    else
        return false;
}

/**
 * Print environment
 */
void GameLauncher::printMap(Environment env) {
    cout << "  ";
    for (int i = 0; i < 9; i++)
        cout << i << " ";
    cout << endl;
    for (int i = 0; i < 9; i++) {
        cout << i << " ";
        for (int j = 0; j < 9; j++) {
            Cell c = env.getCell(i, j);
            if (c.actor != nullptr)
                cout << c.actor->getName() << " ";
            else if (c.path_cell)
                cout << "> ";
            else if ((c.feel_wolf && c.feel_bear) || c.feel_wolf)
                cout << "* ";
            else if (c.feel_bear)
                cout << "+ ";
            else 
                cout << ". ";
        }
        cout << endl;
    }
}

