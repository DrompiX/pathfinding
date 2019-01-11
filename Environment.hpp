#ifndef Environment_hpp
#define Environment_hpp

#include "Agents.hpp"
#include <vector>

struct Cell {
    Agent* actor;
    bool feel_bear;
    bool feel_wolf;
    bool path_cell;
};

const int size = 9;

class Environment {
private:
    RRH *rrh;
    Wolf *wolf;
    Bear *bear;
    WoodCutter *woodcutter;
    Granny *granny;
    Cell forest[size][size];
    vector < pair<unsigned, unsigned> > unused;
public:
    Environment();
    ~Environment();
    Cell& getCell(unsigned, unsigned);
    void setAgent(unsigned, unsigned, Agent*);
    void setBearFeel(unsigned, unsigned);
    void setWolfFeel(unsigned, unsigned);
    void setPathCell(unsigned, unsigned);
    bool checkBounds(unsigned, unsigned);
    void fillEnvironment();
    void addWolf();
    void addBear();
    void addWoodCutter();
    void addGranny();
    bool checkRange(Agent*);
};


#endif
