#include <iostream>
#include <exception>
#include <vector>
#include "grid.h"
#include "agents.h"
#include "simulation.h"

using namespace std;

int main(int argc, char **argv) {
     Simulation sim;

     int hiderExpl = 3, seekerExpl = 2, print = 0;

     if (argc >= 3) {
          hiderExpl = atoi(argv[1]);
          seekerExpl = atoi(argv[2]);
          if (argc == 4) {
               print = 1;
          }
     }

     sim.makeExperiment(hiderExpl, seekerExpl, print);

     return 0;
}