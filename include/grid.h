#ifndef GRID_H
#define GRID_H

#include "agents.h"

void initializeGrid(std::vector<int> &grid);
void printGrid(std::vector<int> grid, Agent hider, Agent seeker);

#endif /* GRID_H */