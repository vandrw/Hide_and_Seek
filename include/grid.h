#ifndef GRID_H
#define GRID_H

#include "agents.h"

void initializeGrid(std::vector<int> &grid);
void initializeRewardsGrid(std::vector<double> &rewards);
void initializeRewardsToBase(std::vector<double> &rewardsToBase, int baseX, int baseY); 
void initializeZerosArray(std::vector<double> &rewards);
void initializeZerosEstimates(std::vector<std::vector<double>> &estimates);
void addDirection(std::vector<int> grid, std::vector<int> &grd, Agent hider, Agent seeker);

void printGrid(std::vector<int> grid, Agent hider, Agent seeker);
void printArray(std::vector<double> array);

#endif /* GRID_H */
