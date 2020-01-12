#ifndef SIMULATION_H
#define SIMULATION_H

#include "agents.h"

struct gameResults {
    int    endTurn           = 0;
    int    hiderFound        = 0;

    double totalRewardHider  = 0.0;   // Total observed reward for the Hider in each game.
    double totalRewardSeeker = 0.0;   // Total observed reward for the Seeker in each game.

    int    hiderFoundTurn    = -1;    // The turn when the hider was found in each game.
    int    wonBySeeker       = 0;     // Value of 0 if the seeker lost and 1 otherwise.
};

class Simulation {
    public:

    // Hyper-parameters
    double epsilon        = 0.5;
    int    hiderAdvantage = 50;

    // Experiment parameters
    int    simPerExperiment   = 5;                    // Number of simulations in an experiment run.
    int    gamesPerSimulation = 100;                    // Number of games in a simulation.
    int    turnsPerGame       = 200;                  // Number of turns in a Game.

    std::vector<int>    grid;                         // The grid where the agents play.

    std::vector<double> rewardsSeeker;                // Rewards for seeker in the grid.
    std::vector<double> rewardsHider;                 // Rewards for hider in the grid.
    std::vector<double> rewardsToBase;                // Rewards that indicate the path to the base.

    // Current iteration
    int simNum  = 0;
    int gameNum = 0;
    int turnNum = 0;

    void makeExperiment();
    void makeSimulation(std::ofstream& logs);
    gameResults  makeGame(Agent hider, Agent seeker);

    void printSimulation(Agent hider, Agent seeker, int hiderFound);
    void printScores(std::ofstream& logs, gameResults gRes);
};

#endif /* SIMULATION_H */
