#ifndef SIMULATION_H
#define SIMULATION_H

#include "agents.h"

struct experimentResults{
    std::vector<double> endTurns;       
    std::vector<double> hiderFoundTurn;
    std::vector<double> hiderFound;
    std::vector<double> hiderRewards;
    std::vector<double> seekerRewards;
    std::vector<double> seekerWins;
};

struct gameResults {
    int    endTurn           = 0;
    int    hiderFound        = 0;

    double totalRewardHider  = 0.0;   // Total observed reward for the Hider in each game.
    double totalRewardSeeker = 0.0;   // Total observed reward for the Seeker in each game.

    int    hiderFoundTurn    = 0;    // The turn when the hider was found in each game.
    int    wonBySeeker       = 0;     // Value of 0 if the seeker lost and 1 otherwise.
};

class Simulation {
    public:

    // Hyper-parameters
    double epsilon        = 0.1;
    int    hiderAdvantage = 50;

    // Experiment parameters
    int    simPerExperiment   = 100;                    // Number of simulations in an experiment run.
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
    void makeSimulation(std::ofstream& logs, experimentResults &eRes);
    gameResults  makeGame(Agent hider, Agent seeker);

    void printSimulation(Agent hider, Agent seeker, int hiderFound);
    void printScoresPerSimulation(std::ofstream& logs, gameResults gRes);
    void adjustSize (experimentResults &eRes);
    void transferGameResults(gameResults gRes, experimentResults &eRes, int i);
    void printExperimentResults(experimentResults eRes);
};

#endif /* SIMULATION_H */
