#include <random>
#include <vector>
#include "agents.h"

using namespace std;

int Agent::decide(double beta, double epsilon, double exploreDegree, std::vector<int> grid){

        // if the agent cannot is blocked, it can't move this turn
        if (isBlocked(grid)) {
            return 4;
        }
        switch (exploration) {
        case 0:    // Random    
            return decideRandomly(grid);      
        case 1:    // Optimistic Initial Values (uses best direction)
            return bestDirection(grid);
        case 2:    // Epsilon-Greedy
            return epsilonGreedy(epsilon, grid);
        case 3:    // Reinforcement Comparison
            return reinforcementComparison(grid);
        case 4:    // Pursuit
            return pursuit(beta, grid);
        case 5:    // UCB
            return UCB(exploreDegree, grid);
    }
}

// Exporation/Expoilation algorithms

int Agent::decideRandomly(std::vector<int> grid){
    random_device generator;
    uniform_int_distribution<int> randInt(0, 4);
    int action;
    if (isBlocked(grid)){   // If agent is blocked, do nothing
            return 4;
    }
    do{ 
        action = randInt(generator);
    }   while(!checkForWall(action, grid));
    return action;   
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

    // encourage seeker to move
    if (type == 1){
        max = values[0];
        direction = 0;            
        // cout << "direction " << 0 << " estimate " << values[0] << " "; 
        for (int i = 1; i < 5; i++){
           // cout << "direction " << i << " estimate " << values[i] << " "; 
            if (max < values[i]){
                max = values[i];
                direction = i;
            }
        }
       // cout << " choice " << direction << "\n";
    // encourage hider to stay
    } else {
        max = values[4];
        direction = 4;
        for (int i = 0; i < 4; i++){
           // cout << "direction " << i << " estimate " << values[i] << " "; 
            if (max < values[i]){
                max = values[i];
                direction = i;
            }
        }
       // cout << "direction " << 4 << " estimate " << values[4] << " "; 
       // cout << " choice " << direction << "\n";
    }
    
    return direction;
}

int Agent::epsilonGreedy(double epsilon, std::vector<int> grid){
    random_device generator;
    uniform_real_distribution<double> randDouble(0,1);

    if (randDouble(generator) > 1-epsilon){
        return decideRandomly(grid);
    }
    return bestDirection(grid);
}

std::vector<double> Agent::computeProbabilities(std::vector<int> grid){
    std::vector<double> probabilities(5, 0.0);
    double sum = 0;

    // only take into account actions that are "legal"
    for (int i=0; i < 5; i++){
        if (checkForWall(i, grid)){
            probabilities[i] = exp(preferences[X*10+Y][i]);
            sum += probabilities[i];
        }else{
            probabilities[i] = -999999;
        }
    }
    for (int i = 0; i < 5; i++){
        probabilities[i] /= sum;
    }
    return probabilities;    
}

int Agent::getMaxIdx(std::vector<double> probabilities){
    // action 4 is always safe to take, especially for hiders
    int idx = 4; 
    int max = probabilities[4];
    for(int i = 0; i < 4; i++){
        if (type == 0 && probabilities[i] > max){
            max = probabilities[i];
            idx = i;
        }
        if ( type == 1 && probabilities[i] >= max ){
            max = probabilities[i];
            idx = i;
        }
    }
    return idx;
}

int Agent::reinforcementComparison(std::vector<int> grid){
    random_device generator;
    uniform_real_distribution<double> randProb(0, 1);
    int choice, bestAction;

    if (isBlocked(grid)){   // If agent is blocked, do nothing
        return 4;
    }

    std::vector<double> probabilities = computeProbabilities(grid);

    bestAction = getMaxIdx(probabilities);

    if ( randProb(generator) < probabilities[bestAction]){
        choice = bestAction;
    }else{
        choice = decideRandomly(grid);
    }
    return choice;
}

void Agent::updatePreferenceBeta(int action, double beta){
    for (int i = 0; i < 5; i++){
        if(i == action){
            preferences[X*10+Y][i] += beta * (1 - preferences[X*10+Y][i]); 
        }else{
            preferences[X*10+Y][i] += beta * (0 - preferences[X*10+Y][i]); 
        }
    }
}


int Agent::pursuit(double beta, std::vector<int> grid){
    random_device generator;
    uniform_real_distribution<double> randProb(0, 1);

    if (isBlocked(grid)){   // If agent is blocked, do nothing
        return 4;
    }

    int maxAction = bestDirection(grid);

    updatePreferenceBeta(maxAction, beta);

    int mostProbableAction = getMaxIdx(preferences[X*10+Y]);
    int choice;

    if (randProb(generator) < preferences[X*10+Y][mostProbableAction]){
        choice = mostProbableAction;
    } else {
        choice = decideRandomly(grid);
    }
    return choice;
}


int Agent::UCB(double exploreDegree, std::vector<int> grid) {
    int dirMaxReward = 4;
    double rewardUCB= 0, maxReward = -9999;
    std::vector<double> values (5, 0);

    if (isBlocked(grid)){   // If agent is blocked, do nothing
        return 4;
    }

    if ( X-1 >= 0 && grid[(X-1)*10 + Y] == 0) {values[0] = estimates[X*10+Y][0];} else {values[0] = -9999999;}
    if ( Y+1 < 10 && grid[X*10 + Y + 1] == 0) {values[1] = estimates[X*10+Y][1];} else {values[1] = -9999999;}
    if ( X+1 < 10 && grid[(X+1)*10 + Y] == 0) {values[2] = estimates[X*10+Y][2];} else {values[2] = -9999999;}
    if ( Y-1 >= 0 && grid[X*10 + Y - 1] == 0) {values[3] = estimates[X*10+Y][3];} else {values[3] = -9999999;}
    values[4] = estimates[X*10 + Y][4];

    for (int i=0; i<5; i++) {
        if (choiceCounter[X*10 + Y][i] == 0) {
            if (checkForWall(i, grid)) { return i;}
        }

        rewardUCB = values[i] 
                + exploreDegree 
                * sqrt(log(counterAll) / choiceCounter[X*10 + Y][i]);

        if (rewardUCB > maxReward) {
            maxReward = rewardUCB;
            dirMaxReward = i;
        }
    }

    return dirMaxReward;
}