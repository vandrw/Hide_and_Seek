#include <iostream>
#include <random>
#include <vector>
#include "agents.h"

using namespace std;

Agent::Agent(int t, std::vector<int> &grid) {
    random_device generator;
    uniform_int_distribution<int> randInt(0, 9);
    uniform_int_distribution<int> randIntDirection(0, 3);

    type = t;                                   // Set agent type
    direction = randIntDirection(generator);    // Set random starting direction
    if (type == 0){ 
        estimates.resize(100);
        for (int i = 0; i < 100; i ++){
            estimates[i].resize(4,0.0);
        } 
    } // Resize array with observed rewards
    if (type == 1){ 
        estimates.resize(100);
        for (int i = 0; i < 100; i ++){
            estimates[i].resize(4,0.0);
        }  
    } // The seeker's array gets initialized with optimistic
                                                // Initial values to encourage exploration

    int x_hider, y_hider;

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
        X = randInt(generator);     // Set appropriate X
        Y = randInt(generator);     // Set appropriate Y
    }
    grid[X * 10 + Y] = type + 2;    // Place agent on grid
}


void Agent::act(int dir, std::vector<int> &grid, double epsilon) {
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
        case 4:         // Do nothing because agent is blocked (by another agent)
            break;     
    }
    // chance the direction in which the agent is looking accordingly
    direction = dir;
    grid[X * 10 + Y] = type + 2;
}

int Agent::checkForWall(int direction, std::vector<int> grid){
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
    }
    return 0;
}

int Agent::isBlocked(std::vector<int> grid){
    if ( !checkForWall(0, grid) && !checkForWall(1, grid) && 
        !checkForWall(2, grid) && !checkForWall(3, grid)){
            return 1;
        } 
    return 0;
}

int Agent::decideRandomly(std::vector<int> grid){
    random_device generator;
    uniform_int_distribution<int> randInt(0, 3);
    int action;
    if (isBlocked(grid)){   // If agent is blocked, do nothing
            return 4;
    }
    do{ 
        action = randInt(generator);
    }   while(!checkForWall(action, grid));
    return action;   
}

int Agent::decide(double epsilon, std::vector<int> grid){
    random_device generator;
    uniform_real_distribution<double> randDouble(0,1);
    uniform_int_distribution<int> randInt(0, 3);
    int decision;
    if (isBlocked(grid)) {
        return 4;
    }
    if (randDouble(generator) > 1-epsilon){
        return decideRandomly(grid);
    }
    return bestDirection(grid);
}

int Agent::findAgentNorth(std::vector<int> grid){
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

int Agent::findAgentSouth(std::vector<int> grid){
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
    // Check third row
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

int Agent::findAgentEast(std::vector<int> grid){
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

int Agent::findAgentWest(std::vector<int> grid){
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

int Agent::findAgent(std::vector<int> grid){
    switch (direction) {
        case 0:         // NORTH
            return findAgentNorth(grid);      
        case 1:         // EAST
            return findAgentEast(grid);
        case 2:         // SOUTH
            return findAgentSouth(grid);
        case 3:         // WEST
            return findAgentWest(grid);
        default:
            return 0;
    }
}

int Agent::bestDirection(std::vector<int> grid){
    int direction;
    double max;
    std::vector<double> values (5, 0);

    if ( X-1 >= 0 && grid[(X-1)*10 + Y] == 0) {values[0] = estimates[X*10+Y][0];} else {values[0] = -10000;}
    if ( Y+1 < 10 && grid[X*10 + Y + 1] == 0) {values[1] = estimates[X*10+Y][1];} else {values[1] = -10000;}
    if ( X+1 < 10 && grid[(X+1)*10 + Y] == 0) {values[2] = estimates[X*10+Y][2];} else {values[2] = -10000;}
    if ( Y-1 >= 0 && grid[X*10 + Y - 1] == 0) {values[3] = estimates[X*10+Y][3];} else {values[3] = -10000;}
    values[4] = estimates[X*10 + Y][4];

    max = values[4];
    direction = 4;
    for (int i = 0; i < 4; i++){
        if (max < values[i]){
            max = values[i];
            direction = i;
        }
    }

    return direction;

}


int Agent::playTurn(double epsilon, std::vector<int> &grid){
    int action;
    if (type == 0){ // Hider random
        action = decideRandomly(grid);
    }else{
        action = decide(epsilon, grid);
    }
    act(action, grid, epsilon);
    return findAgent(grid);
}


double Agent::getReward(std::vector<double> rewards, int turn, int hiderFoundTurn){
    random_device generator;
    uniform_real_distribution<double> randDouble(-1, 1);
    double newReward;
   
    if (hiderFoundTurn == 0){
        // before the hider was discovered
        if (type == 0){
            // hiders get the tile reward + 1 for every turn they are not discovered 
            newReward = rewards[X*10+Y] + randDouble(generator) + 1;
        }else{
            // seekers get the tile reward - 1 for every turn until they dicover the hider
            newReward = rewards[X*10+Y] + randDouble(generator) - 1;
        }
    }else if (turn == hiderFoundTurn){
        // in the turn the hider was found
        if (type == 0){
            // hider gets a penalty 
            newReward = rewards[X*10+Y] + randDouble(generator) - 10;
        }else{
            // seeker gets bonus 
            newReward = rewards[X*10+Y] + randDouble(generator) + 10;
        }
    }else{
        // after the hider was found, agents receive the rewards according to their location
        newReward = rewards[X*10+Y] + randDouble(generator);
    }
    // For SARSA estimates are updated later in makeGame(...)
    //estimates[X*10+Y] += (double)(1.0/(turn+1.0)) * (newReward - estimates[X*10+Y]);
    //printEstimates();
    return newReward;
}

void Agent::updateEstimates(double reward, double alpha, double gamma, double epsilon, std::vector<int> grid){
    int nextAction = bestDirection(grid);
    double nextEstimate = estimates[X*10+Y][decide(epsilon, grid)];
    estimates[X*10+Y][direction] += alpha * (reward + gamma * nextEstimate - estimates[X*10+Y][direction]);
    // cout << "newReward = " << reward << ", ";
    // cout << "next estimate = " << nextEstimate << ", ";
    // cout << "new estimate = " << estimates[X*10+Y][direction] << "\n";
}

 void Agent::printEstimates(){
    for (int i = 0;i<100;i++){
        for(int j=0; j<4; j++){
            cout << estimates[i][j] << " ";
        }
        cout << "\n";
    }
    cout <<"\n";
}

int Agent::getX_Coord() {
    return X;
}

int Agent::getY_Coord() {
    return Y;
}

int Agent::getDirection(){
    return direction;
}

void Agent::setX_Coord(int x_coord) {
    X = x_coord;
}

void Agent::setY_Coord(int y_coord) {
    Y = y_coord;
}

int Agent::hasWon(int baseX, int baseY){
    if (X == baseX && Y == baseY && discovered == 1){
        return 1;
    }
    return 0;
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