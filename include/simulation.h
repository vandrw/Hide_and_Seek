#ifndef SIMULATION_H
#define SIMULATION_H

#include "agents.h"

// int S = 100    // Number of simulations in an experiment run
// int G = 100    // Number of games in a simulation
// int T = 200;   // Number of turns in a Game.

int makeGame(int turnsPerGame, std::vector<int> &hiderScores, std::vector<int> &seekerScores, std::vector<int> &grid, std::vector<double> rewardsHider, std::vector<double> rewardsSeeker , std::vector<double> rewardsToBase, Agent hider, Agent seeker);
void printSimulation(std::vector<int> grid, Agent hider, Agent seeker, int turn);
void makeSimulation(int turnsPerGame, int gamesPerSimulation, std::ofstream& logs);
void printScores(std::ofstream& logs, int game, int endTurn, std::vector<double> hiderScores, std::vector<double> seekerScores);

#endif /* SIMULATION_H */
