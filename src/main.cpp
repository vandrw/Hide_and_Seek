#include <iostream>
#include <exception>
#include <vector>
#include "grid.h"
#include "agents.h"
#include "simulation.h"

using namespace std;

// Whole experiment is for X simulations
// Simulation has 100 games with the same starting location and hider seeker observations
// Each game has 300 turns

int main(int argc, char **argv) {
     Simulation sim;

     sim.makeExperiment();

     return 0;
}