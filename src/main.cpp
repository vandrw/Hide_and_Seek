#include <iostream>
#include <vector>
#include "grid.h"
#include "agents.h"
#include "simulation.h"
#include <exception>
#include <iostream>
#include <fstream>

using namespace std;

// Whole experiment is for X simulations
// Simulation has 100 games with the same starting location and hider seeker observations
// Each game has 200 turns

int main(int argc, char **argv) {
     ofstream logs;
     logs.open ("logs.csv");
     logs <<"Game\t End Turn\t Outcome\t Scores \n";
     makeSimulation(logs);
     logs.close();
}