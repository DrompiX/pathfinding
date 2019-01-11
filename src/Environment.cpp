#include "Environment.hpp"
#include <iostream>
#include <vector>

Environment::Environment() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            forest[i][j].feel_bear = false;
            forest[i][j].feel_wolf = false;
            forest[i][j].path_cell = false;
            forest[i][j].actor = nullptr;
        }
}

Cell& Environment::getCell(unsigned i, unsigned j) {
    return forest[i][j];
}

void Environment::setAgent(unsigned i, unsigned j, Agent* a) {
    forest[i][j].actor = a;
    if (a != nullptr) a->setPosition(i, j);
}

void Environment::setBearFeel(unsigned i, unsigned j) {
    forest[i][j].feel_bear = true;
}

void Environment::setWolfFeel(unsigned i, unsigned j) {
    forest[i][j].feel_wolf = true;
}

void Environment::setPathCell(unsigned i, unsigned j) {
    forest[i][j].path_cell = true;
}

bool Environment::checkBounds(unsigned i, unsigned j) {
    if (i < SIZE && j < SIZE)
        return true;
    else
        return false;
}

void Environment::fillEnvironment() {
    rrh = new RRH(0, 0);
    setAgent(0, 0, rrh);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (!(i == 0 && j == 0))
                unused.push_back(make_pair(i, j));
    addWolf();
    addBear();
    addWoodCutter();
    addGranny();
}

void Environment::addWolf() {
    int wolf_cell = rand() % unused.size();
    unsigned wx = unused[wolf_cell].first;
    unsigned wy = unused[wolf_cell].second;
    wolf = new Wolf(wx, wy);
    // If wolf's range on RRH - try again
    if (!checkRange(wolf)) { addWolf(); return;}
    setAgent(wx, wy, wolf);
    unused.erase(unused.begin() + wolf_cell);

    if (checkBounds(wx - 1, wy)) setWolfFeel(wx - 1, wy);
    if (checkBounds(wx + 1, wy)) setWolfFeel(wx + 1, wy);
    if (checkBounds(wx, wy - 1)) setWolfFeel(wx, wy - 1);
    if (checkBounds(wx, wy + 1)) setWolfFeel(wx, wy + 1);
}

void Environment::addBear() {
    int bear_cell = rand() % unused.size();
    int bx = unused[bear_cell].first;
    int by = unused[bear_cell].second;
    bear = new Bear(bx, by);
    // If Bear's range on RRH - try again
    if (!checkRange(bear)) {addBear(); return;}
    setAgent(bx, by, bear);
    setBearFeel(bx, by);
    unused.erase(unused.begin() + bear_cell);
    
   for (int i = bx - 1; i <= bx + 1; i++)
        for (int j = by - 1; j <= by + 1; j++)
            if (checkBounds(i, j) && !(i == bx && j == by))
                setBearFeel(i, j);

    for (int i = 0; i < (int)unused.size(); i++) {
        unsigned x = unused[i].first;
        unsigned y = unused[i].second;
        if (forest[x][y].feel_wolf || forest[x][y].feel_bear) {
            unused.erase(unused.begin() + i);
            i--;
        }
    }
}

void Environment::addWoodCutter() {
    // Random location of WC's home
    int wc_home_cell = rand() % unused.size();
    unsigned hx = unused[wc_home_cell].first;
    unsigned hy = unused[wc_home_cell].second;
    unused.erase(unused.begin() + wc_home_cell);
 
    // Random location of WC's work
    int wc_work_cell = rand() % unused.size();
    unsigned wx = unused[wc_work_cell].first;
    unsigned wy = unused[wc_work_cell].second;
    unused.erase(unused.begin() + wc_work_cell);

    // Place WC either in home cell or work cell
    int choice = rand() % 2;
    woodcutter = new WoodCutter(hx, hy, wx, wy, choice);

    if (choice)
        setAgent(hx, hy, woodcutter);
    else
        setAgent(wx, wy, woodcutter);

    point home = {hx, hy};
    point work = {wx, wy};
    rrh->map_data.woodcutter = make_pair(home, work);
}

void Environment::addGranny() {
    int gr_cell = rand() % unused.size();
    unsigned gx = unused[gr_cell].first;
    unsigned gy = unused[gr_cell].second;
    granny = new Granny(gx, gy);
    setAgent(gx, gy, granny);
    unused.erase(unused.begin() + gr_cell);

    point gr = {gx, gy};
    rrh->map_data.granny = gr;
}

bool Environment::checkRange(Agent* a) {
    Bear* b = dynamic_cast<Bear*>(a);
    if (b != nullptr) {
        int x = b->getPosition().x;
        int y = b->getPosition().y;
        for (int i = x - 1; i <= x + 1; i++)
            for (int j = y - 1; j <= y + 1; j++)
                if (i == 0 && j == 0)
                    return false;
        return true;
    }
    Wolf* w = dynamic_cast<Wolf*>(a);
    if (w != nullptr) {
        int x = w->getPosition().x;
        int y = w->getPosition().y; 
        if ((x - 1 == 0 && y == 0) || (x == 0 && y - 1 == 0))
            return false;
        return true;
    }
    return true;
}

