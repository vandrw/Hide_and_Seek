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
// Each game has 300 turns

int main(int argc, char **argv) {
     ofstream logs;

     int turnsPerGame = 300;
     int gamesPerSimulation = 5;
     
     logs.open ("bin/logs.csv");
     logs <<"Game, Agent, End Turn, Winner, Hider Discovered, Scores\n";
     
     makeSimulation(turnsPerGame, gamesPerSimulation, logs);
     
     logs.close();

     return 0;
}