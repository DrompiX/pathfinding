#ifndef Agents_hpp
#define Agents_hpp

#include <string>

using namespace std;

struct point {
    int x, y;
};

class Agent {
protected:
    string name;
    point position;
    
public:
    string getName();
    void setPosition(unsigned x, unsigned y);
    point getPosition() { return position; };
    virtual void do_action();
};

struct knowledge {
    point bear;
    point wolf;
    point granny;
    // locations: <House, Work>
    pair <point, point> woodcutter;
    bool wcVisited;
};

class RRH: public Agent {
    int berries;
public:
    knowledge map_data;
    RRH(unsigned x, unsigned y);
    int getBerries();
    void setBerries(unsigned new_amount);
    void do_action();
};

class Granny: public Agent {
public:
    Granny(unsigned x, unsigned y);
    void do_action();
};

class Wolf: public Agent {
public:
    Wolf(unsigned x, unsigned y);
    int getRange();
    void do_action();
};

class Bear: public Agent {
public:
    Bear(unsigned x, unsigned y);
    int getRange();
    void do_action();
};

class WoodCutter: public Agent {
private:
    // <House, Work>
    pair <point, point> locations;  
public:
    WoodCutter(unsigned hx, unsigned hy, 
            unsigned wx, unsigned wy, bool h);
    void do_action();
};

#endif /* Agents_hpp */
