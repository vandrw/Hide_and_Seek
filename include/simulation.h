#ifndef SIMULATION_H
#define SIMULATION_H

#include "agents.h"

// int N = 1;     // Number of plays in a game.
// int P = 150;   // Number of turns in a play.

void makeGame(std::vector<int> grid, Agent hider, Agent seeker);
void makePlay(std::vector<int> grid, Agent hider, Agent seeker);
void printSimulation(std::vector<int> grid, Agent hider, Agent seeker);

#endif /* SIMULATION_H */
