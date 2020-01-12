#ifndef GRID_H
#define GRID_H

#include "agents.h"

void initializeGrid(std::vector<int> &grid);
void initializeRewardsGrid(std::vector<double> &rewards);
void initializeRewardsToBase(std::vector<double> &rewardsToBase, int baseX, int baseY); 

void addDirection(std::vector<int> grid, std::vector<int> &grd, Agent hider, Agent seeker);

void printGrid(std::vector<int> grid, Agent hider, Agent seeker);

#endif /* GRID_H */
