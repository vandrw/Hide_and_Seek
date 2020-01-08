#include <random>
#include <vector>
#include <iostream>
#include "agents.h"

using namespace std;

Agent::Agent(int t, std::vector<int> &grid) {
    random_device generator;
    uniform_int_distribution<int> randInt(0, 9);

    type = t;

    X = randInt(generator);
    Y = randInt(generator);

    while (grid[X * 10 + Y] != 0) {
        X = randInt(generator);
        Y = randInt(generator);
    }

    grid[X * 10 + Y] = type + 2;
}

void Agent::move(int dir, std::vector<int> &grid) {
    // Setting the old coordinate to empty.
    grid[X * 10 + Y] = 0;

    switch (dir) {
        case 0:         // NORTH
            if ( (X - 1 >= 0) && 
                 (grid[(X-1) * 10 + Y] == 0) 
               )   { X--; }
            break;
        case 1:         // EAST
            if ( (Y + 1 < 10) &&
                 (grid[X * 10 + Y + 1] == 0)
               ) { Y++; }
            break;
        case 2:         // SOUTH
            if ( (X + 1 < 10) &&
                 (grid[(X +1) * 10 + Y] == 0) 
               ) { X++; }
            break;
        case 3:         // WEST
            if ( (Y - 1 >= 0) &&
                 (grid[X * 10 + Y - 1] == 0)
               ) { Y--;}
            break;
        case 4:         // NOTHING
            break;
    }

    grid[X * 10 + Y] = type + 2;
}

int Agent::getX_Coord() {
    return X;
}

int Agent::getY_Coord() {
    return Y;
}

void Agent::setX_Coord(int x_coord) {
    X = x_coord;
}

void Agent::setY_Coord(int y_coord) {
    Y = y_coord;
}

void Agent::printCoords() {
    cout << "The ";
    if (type == 0) {
        cout << "hider ";
    } else {
        cout << "seeker ";
    }
    
    cout << "is located at (" 
         << getX_Coord() 
         << ", " << getY_Coord() 
         << ").\n";
}