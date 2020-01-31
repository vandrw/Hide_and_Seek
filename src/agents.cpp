#include <iostream>
#include <random>
#include <vector>
#include "agents.h"

using namespace std;

Agent::Agent(int t, int expStrategy, double initialValue) {
    exploration = expStrategy;
    type = t;                                    // Set agent type
    direction = 4;                               // Default starting direction

    if (expStrategy != 1) {
        initialValue = 0;
    }

    meanRewards.resize(100, 0.0);
    estimates.resize(100);
    preferences.resize(100);
    choiceCounter.resize(100);

    for (int i = 0; i < 100; i ++){
        estimates[i].resize(5, initialValue);
        preferences[i].resize(5, 0.0);
        choiceCounter[i].resize(5, 1);
    }  
}

void Agent::reinitialize(double initialValue, std::vector<int> &grid) {
    random_device generator;
    uniform_int_distribution<int> randInt(0, 9);

    direction = 4;                               // Default starting direction
    counterAll = 0;

    if (exploration != 1) {
        initialValue = 0;
    } 

    std::fill(meanRewards.begin(), meanRewards.end(), 0.0);

    for (int i = 0; i < 100; i ++){
        std::fill(choiceCounter[i].begin(), choiceCounter[i].end(), 1);
        std::fill(estimates[i].begin(), estimates[i].end(), initialValue);
        std::fill(preferences[i].begin(), preferences[i].end(), 0.0);
    } 

    int x_hider = 0, y_hider = 0;

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

        X = randInt(generator);
        Y = randInt(generator);

        while (grid[X * 10 + Y] != 0 || abs(X-x_hider) < 4 || abs(Y-y_hider) < 4) {
            X = randInt(generator);     // Set appropriate X
            Y = randInt(generator);     // Set appropriate Y
        }
    } else {
        X = randInt(generator);
        Y = randInt(generator);

        while (grid[X * 10 + Y] != 0) {
            X = randInt(generator);     // Set appropriate X
            Y = randInt(generator);     // Set appropriate Y
        }
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

    // Change the direction in which the agent is looking
    direction = dir;

    choiceCounter[X * 10 + Y][direction]++;
    counterAll++;
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
        case 4:         // Agent doesn't move, so it is not blocked by a wall
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

int Agent::findAgentAllDirections(std::vector<int> grid){
    int found = 0;
    for (int i = 0; i < 4; i++){
        direction = i;
        found = findAgent(grid);
        if (found != 0){
            break;
        } 
    }
    direction = 4;
    return found;
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
        case 4:
            return findAgentAllDirections(grid);
        default:
            return 0;
    }
}


int Agent::playTurn(double beta, double epsilon, double exploreDegree, std::vector<int> &grid){
    int action;
    action = decide(beta, epsilon, exploreDegree, grid);
    act(action, grid, epsilon);
    // at the end of the turn, the agent looks for the other agent
    return findAgent(grid);
}


double Agent::getReward(std::vector<double> rewards, int turn, int hiderFoundTurn, int maxTurn){
    random_device generator;
    uniform_real_distribution<double> randDouble(-1, 1);
    double newReward;
    
    if (discovered == 0){
        // before the hider was discovered
        if (type == 0){
            // hiders get the tile reward + 1 for every turn they are not discovered 
            newReward = rewards[X*10+Y] + randDouble(generator) + 1;
        } else {
            // seekers get the tile reward - 1 for every turn until they dicover the hider
            newReward = rewards[X*10+Y] + randDouble(generator) - 1;
        }
     }else if (turn == hiderFoundTurn && discovered == 1){
        // in the turn the hider was found
        if (type == 0){
            // hider gets a penalty 
            newReward = rewards[X*10+Y] + randDouble(generator) - 10;
        } else {
            // seeker gets bonus 
            newReward = rewards[X*10+Y] + randDouble(generator) + 10;
        }
    } else if(turn == maxTurn-1 && discovered == 1 && type == 1){
        newReward = rewards[X*10+Y] + randDouble(generator) + 10;
    } else if (turn == maxTurn-1 && discovered == 0 && type == 0){
        newReward = rewards[X*10+Y] + randDouble(generator) + 10;
    } else {
        // after the hider was found, agents receive the rewards according to their location
        newReward = rewards[X*10+Y] + randDouble(generator);
    }
    
    return newReward;
}

void Agent::updateMeanReward(double alphaExp, double newReward){
    meanRewards[X*10+Y] += alphaExp * (newReward - meanRewards[X*10+Y]);
}

void Agent::updateEstimates(double reward, double alpha, double gamma, 
                            double epsilon, double alphaExp, double beta,
                            double exploreDegree, std::vector<int> grid) {
    
    int nextAction = decide(beta, epsilon, exploreDegree, grid);
    double nextEstimate = estimates[X*10+Y][nextAction];
    
    if (direction == 0){
        estimates[(X+1)*10+Y][direction] += alpha * (reward + gamma * nextEstimate - estimates[(X+1)*10+Y][direction]);
        if (exploration == 3){ 
            preferences[(X+1)*10+Y][direction] += (double)1/5 * (reward - meanRewards[(X+1)*10+Y]);
        }
    }
    if (direction == 1){
        estimates[X*10+Y-1][direction] += alpha * (reward + gamma * nextEstimate - estimates[X*10+Y-1][direction]);
        if (exploration == 3){ 
            preferences[X*10+Y-1][direction] += (double)1/5 * (reward - meanRewards[X*10+Y-1]);
        }
    }
    if (direction == 2){
        estimates[(X-1)*10+Y][direction] += alpha * (reward + gamma * nextEstimate - estimates[(X-1)*10+Y][direction]);
        if (exploration == 3){ 
            preferences[(X-1)*10+Y][direction] += (double)1/5 * (reward - meanRewards[(X-1)*10+Y]);
        }
    }
    if (direction == 3){
        estimates[X*10+Y+1][direction] += alpha * (reward + gamma * nextEstimate - estimates[X*10+Y+1][direction]);
        if (exploration == 3){ 
            preferences[X*10+Y+1][direction] += (double)1/5 * (reward - meanRewards[X*10+Y+1]);
        }
    }
    if (direction == 4){
        estimates[X*10+Y][direction] += alpha * (reward + gamma * nextEstimate - estimates[X*10+Y][direction]);
        if (exploration == 3){ 
            preferences[X*10+Y][direction] += (double)1/5 * (reward - meanRewards[X*10+Y]);
        }
    }
    if (exploration == 3){
        updateMeanReward(alphaExp, reward);
    }
}

 void Agent::printEstimates(){
    for (int i = 0;i<100;i++){
        for(int j=0; j<5; j++){
            cout << estimates[i][j] << " ";
        }
        cout << "\n";
    }
    cout <<"\n";
}

 void Agent::printEstimatesMean(){
    double sum;
    for (int k = 0; k < 10; k++ ){
        for (int i = 0;i<10;i++){
            sum = 0;
            for(int j=0; j<5; j++){
                sum += estimates[k*10+i][j];
            }
            cout << sum/5 << " ";
        }
        cout<< "\n";
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