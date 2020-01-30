#ifndef SIMULATION_H
#define SIMULATION_H

#include "agents.h"

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
    double epsilon        = 0.1; // Epsilon-Greedy
    double alpha          = 0.1; // Learning rate for Q-learning
    double gamma          = 0.9; // Q-learning discount factor.
    double beta           = 0.1; // Pursuit
    double initialValue   = 5;   // Optimistic Initial Values
    double alphaExp       = 0.2; // Used as parameter for Reinforcement Comparison
    double exploreDegree  = 2.5;   // UCB

    // Experiment parameters
    int    simPerExperiment   = 500;                  // Number of simulations in an experiment run.
    int    gamesPerSimulation = 1500;                 // Number of games in a simulation.
    int    turnsPerGame       = 200;                  // Number of turns in a Game.
    int    hiderAdvantage     = 50;

    std::vector<int>    grid;                         // The grid where the agents play.
    std::vector<double> rewardsSeeker;                // Rewards for seeker in the grid.
    std::vector<double> rewardsHider;                 // Rewards for hider in the grid.
    std::vector<double> rewardsToBaseHider;           // Rewards that indicate the path to the base.
    std::vector<double> rewardsToBaseSeeker;

    // Current iteration
    int simNum  = 0;
    int gameNum = 0;
    int turnNum = 0;

    void makeExperiment(int hiderStrat, int seekerStrat);
    void makeSimulation(Agent hider, Agent seeker, std::ofstream& logs);
    gameResults  makeGame(Agent &hider, Agent &seeker);

    void printSimulation(Agent hider, Agent seeker, int hiderFound, int baseX, int baseY);
    void printScoresPerSimulation(std::ofstream& logs, gameResults gRes);

    void createLogs(std::ofstream& logs, int hiderStrat, int seekerStrat);
    void printCurrentExperiment(int hiderStrat, int seekerStrat);
};

#endif /* SIMULATION_H */
