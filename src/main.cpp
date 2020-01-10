#include <iostream>
#include <vector>
#include "grid.h"
#include "agents.h"
#include "simulation.h"
#include <exception>
using namespace std;

int main(int argc, char **argv) {
     std::vector<int> grid(100, 0);

     initializeGrid(grid);
     
     Agent  hider(0, grid);
     Agent seeker(1, grid);

     printSimulation(grid, hider, seeker);

     makeGame(grid, hider, seeker);
}