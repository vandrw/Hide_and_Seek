#ifndef SIMULATION_H
#define SIMULATION_H

#include "agents.h"

// int S = 100    // Number of simulations in an experiment run
// int G = 100    // Number of games in a simulation
// int T = 200;   // Number of turns in a Game.

void makeGame(std::vector<int> grid, std::vector<double> rewardsHider, std::vector<double> rewardsSeeker , std::vector<double> rewardsToBase, Agent hider, Agent seeker);
void printSimulation(std::vector<int> grid, Agent hider, Agent seeker);
void makeSimulation();

#endif /* SIMULATION_H */
