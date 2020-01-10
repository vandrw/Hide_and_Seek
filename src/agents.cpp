#include <random>
#include <vector>
#include <iostream>
#include "agents.h"

using namespace std;

Agent::Agent(int t, std::vector<int> &grid) {
    random_device generator;
    uniform_int_distribution<int> randInt(0, 9);
    
    type = t;
    
    int x_hider, y_hider;
    //estimates = std::vector<int> (100, 0);

    // If placing the second agent, it should be out of the sight of the first agent
    if (type == 1){
        for (int x = 0; x<10; x++){
            for (int y = 0; y<10; y++){
                if (grid[x*10 + y] == 2){
                    x_hider = x;
                    y_hider = y;
                    break;
                } 
            }
        }
    }

    X = randInt(generator);
    Y = randInt(generator);

    while (grid[X * 10 + Y] != 0 || abs(X-x_hider) < 4 || abs(Y-y_hider) <4) {
        X = randInt(generator);
        Y = randInt(generator);
    }
    grid[X * 10 + Y] = type + 2;
}

void Agent::act(int dir, std::vector<int> &grid) {
    // Setting the old coordinate to empty.
    grid[X * 10 + Y] = 0;
    // Compute new coordinates
    switch (dir) {
        case 0:         // NORTH
            X--;
            break;
        case 1:         // EAST
            Y++;
            break;
        case 2:         // SOUTH
            X++;
            break;
        case 3:         // WEST
            Y--;
            break;
        case 4:
            // In this case the agent will look in all directions and choose a new direction for the next turn      
            dir = look_around(grid);
            break;     
    }
    // chance the direction in which the agent is looking accordingly
    direction = dir;
    grid[X * 10 + Y] = type + 2;
}

int Agent::check_for_wall(int direction, std::vector<int> &grid){
    switch (direction) {
        case 0:         // NORTH
            if ( (X - 1 >= 0) && 
                 (grid[(X-1) * 10 + Y] == 0) 
               )   { return 1; }
            break;
        case 1:         // EAST
            if ( (Y + 1 < 10) &&
                 (grid[X * 10 + Y + 1] == 0)
               ) { return 1; }
            break;
        case 2:         // SOUTH
            if ( (X + 1 < 10) &&
                 (grid[(X +1) * 10 + Y] == 0) 
               ) { return 1; }
            break;
        case 3:         // WEST
            if ( (Y - 1 >= 0) &&
                 (grid[X * 10 + Y - 1] == 0)
               ) { return 1;}
            break;
        return 0;
    }
}

int Agent::decide(double epsilon, std::vector<int> &grid){
    random_device generator;
    uniform_real_distribution<double> randDouble(0, 1);
    int decision;
    // Decide to change direction with a probability of epsilon, else keep direction
    if (randDouble(generator) > 1 - epsilon){
        decision = 4;
    }else{
        decision = direction;
        // If there is a wall in the chosen direction, then look around
        if(!check_for_wall(direction, grid)){
            decision = 4;
        }
    }
    return decision;
}

int Agent::find_agent_north(std::vector<int> &grid){
    // There is a wall ahead or the end of the maze => can't see agents there
    if (X - 1 < 0 || grid[(X-1) * 10 + Y] == 1) { return 0; }
    // Sees agent ahead
    if (grid[(X-1) * 10 + Y] == 3 - type){ return 1; }
    // No second row to check
    if ( X-2 < 0){ return 0; } 
    // Check second row
    for (int y = Y-1; y <= Y + 1; y ++){
        if ( y >= 0 && y < 10) {
            //found agent
            if (grid[(X-2)*10 + y] == 3 - type) { return 1;}
        }
    }
    // No third row to check
    if (X-3 < 0 ){ return 0;}
    // Check thrird row
    for (int y = Y-2; y <= Y + 2; y++){
        // If agent found
        if (y >= 0 && y < 10 && grid[(X-3)*10+y] == 3-type) {
        // And there's a wall in front of it in the field of view return 0   
            if ( y!= Y-2 && y!= Y+2 && grid[(X-2)*10+y] == 1) { return 0; }
            else if (y == Y-2 && grid[(X-2)*10+y-1] == 1) { return 0; }
            else if (y == Y+2 && grid[(X-2)*10+y+1] == 1) { return 0; }
            return 1;
        }
    }
    return 0;   
}

int Agent::find_agent_south(std::vector<int> &grid){
    // There is a wall ahead or the end of the maze => can't see agents there
    if (X + 1 < 0 || grid[(X+1) * 10 + Y] == 1) { return 0; }
    // Sees agent ahead
    if (grid[(X+1) * 10 + Y] == 3 - type){ return 1; }
    // No second row to check
    if ( X+2 >= 10){ return 0; } 
    // Check second row
    for (int y = Y-1; y <= Y + 1; y ++){
        if ( y >= 0 && y < 10) {
            //found agent
            if (grid[(X+2)*10 + y] == 3 - type) { return 1;}
        }
    }
    // No third row to check
    if (X+3 >= 10 ){ return 0;}
    // Check thrird row
    for (int y = Y-2; y <= Y + 2; y++){
        // If agent found
        if (y >= 0 && y < 10 && grid[(X+3)*10+y] == 3-type) {
        // And there's a wall in front of it in the field of view return 0   
            if ( y!= Y-2 && y!= Y+2 && grid[(X+2)*10+y] == 1) { return 0; }
            else if (y == Y-2 && grid[(X+2)*10+y-1] == 1) { return 0; }
            else if (y == Y+2 && grid[(X+2)*10+y+1] == 1) { return 0; }
            return 1;
        }
    }
    return 0;   
}

int Agent::find_agent_east(std::vector<int> &grid){
    // There is a wall ahead or the end of the maze => can't see agents there
    if (Y + 1 < 0 || grid[X * 10 + Y+1] == 1) { return 0; }
    // Sees agent ahead
    if (grid[X * 10 + Y+1] == 3 - type){ return 1; }
    // No second row to check
    if ( Y+2 >= 10){ return 0; } 
    // Check second row
    for (int x = X-1; x <= X + 1; x ++){
        if ( x >= 0 && x < 10) {
            //found agent
            if (grid[X*10 + Y+2] == 3 - type) { return 1;}
        }
    }
    // No third row to check
    if (Y+3 >= 10 ){ return 0;}
    // Check thrird row
    for (int x = X-2; x <= X + 2; x++){
        // If agent found
        if (x >= 0 && x <= 10 && grid[x*10+Y+3] == 3-type) {
        // And there's a wall in front of it in the field of view return 0   
            if ( x!= X-2 && x!= X+2 && grid[(x-1)*10+Y+2] == 1) { return 0; }
            else if (x == X-2 && grid[(x-1)*10+Y+2] == 1) { return 0; }
            else if (x == X+2 && grid[(x-1)*10+Y+2] == 1) { return 0; }
            return 1;
        }
    }
    return 0;   
}

int Agent::find_agent_west(std::vector<int> &grid){
    // There is a wall ahead or the end of the maze => can't see agents there
    if (Y - 1 < 0 || grid[X * 10 + Y-1] == 1) { return 0; }
    // Sees agent ahead
    if (grid[X * 10 + Y-1] == 3 - type){ return 1; }
    // No second row to check
    if ( Y-2 >= 10){ return 0; } 
    // Check second row
    for (int x = X - 1; x <= X + 1; x ++){
        if ( x >= 0 && x < 10) {
            //found agent
            if (grid[X*10 + Y-2] == 3 - type) { return 1;}
        }
    }
    // No third row to check
    if (Y-3 < 0 ){ return 0;}
    // Check thrird row
    for (int x = X-2; x <= X + 2; x++){
        // If agent found
        if (x >= 0 && x <= 10 && grid[x*10+Y-3] == 3-type) {
        // And there's a wall in front of it in the field of view return 0   
            if ( x!= X-2 && x!= X+2 && grid[(x-1)*10+Y-2] == 1) { return 0; }
            else if (x == X-2 && grid[(x-1)*10+Y-2] == 1) { return 0; }
            else if (x == X+2 && grid[(x-1)*10+Y-2] == 1) { return 0; }
            return 1;
        }
    }
    return 0;   
}

int Agent::find_agent(std::vector<int> &grid){
    switch (direction) {
        case 0:         // NORTH
            return find_agent_north(grid);      
        case 1:         // EAST
            return find_agent_east(grid);
        case 2:         // SOUTH
            return find_agent_south(grid);
        case 3:         // WEST
            return find_agent_west(grid);
        default:
            return 0;
    }
}

int Agent::look_around(std::vector<int> &grid) {
    //int best_direction = bestEstimate();
    random_device generator;
    uniform_int_distribution<int> randInt(0, 3);
    return randInt(generator);
}

int Agent::play_turn(double epsilon, std::vector<int> &grid){
    int action = decide(epsilon, grid);
    act(action, grid);
    return find_agent(grid);
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
    
    cout << "is located at (" << getX_Coord() << ", " << getY_Coord() << ").\n";
}