#include <iostream>
#include <fstream>

#include <string>
#include <random>
#include <vector>
#include <thread>
#include <chrono>

#include "agents.h"
#include "grid.h"
#include "simulation.h"

using namespace std;

void Simulation::makeExperiment(int hiderStrat, int seekerStrat) {
    ofstream logs;

    // Initializing agent and environment vectors.
    Agent  hider(0, hiderStrat, initialValue);
    Agent seeker(1, seekerStrat, initialValue);

    rewardsSeeker.resize(100, 0.0);
    rewardsHider.resize(100, 0.0);
    rewardsToBaseSeeker.resize(100, 0.0);
    rewardsToBaseHider.resize(100, 0.0);
    grid.resize(100, 0);

    printCurrentExperiment(hiderStrat, seekerStrat);

    createLogs(logs, hiderStrat, seekerStrat);

    for (int i=0; i < simPerExperiment; i++) {
        simNum++;
        makeSimulation(hider, seeker, logs);
    }

    logs.close();
}


void Simulation::makeSimulation(Agent hider, Agent seeker, std::ofstream& logs) {
    gameResults gRes;

    std::fill(rewardsSeeker.begin(), rewardsSeeker.end(), 0.0);
    std::fill(rewardsHider.begin(), rewardsHider.end(), 0.0);
    std::fill(rewardsToBaseSeeker.begin(), rewardsToBaseSeeker.end(), 0.0);
    std::fill(rewardsToBaseHider.begin(), rewardsToBaseHider.end(), 0.0);

    hider.reinitialize(initialValue, grid);
    seeker.reinitialize(initialValue, grid);

    initializeZerosArray(rewardsSeeker);
    initializeGrid(grid);
    initializeRewardsGrid(rewardsHider);

    int baseX  = seeker.getX_Coord();
    int baseY  = seeker.getY_Coord();
    int hiderX = hider.getX_Coord();
    int hiderY = hider.getY_Coord();

    initializeRewardsToBase(rewardsToBaseHider, baseX, baseY);
    initializeRewardsToBase(rewardsToBaseSeeker, baseX, baseY);

    for (int i = 0; i < gamesPerSimulation; i ++ ) {
        gameNum++;

        gRes = makeGame(hider, seeker);

        printScoresPerSimulation(logs, gRes);

        // (Re-)set starting locations, grid & scores for next game
        initializeGrid(grid);

        hider.setX_Coord(hiderX);
        hider.setY_Coord(hiderY);

        seeker.setX_Coord(baseX);
        seeker.setY_Coord(baseY);

        seeker.discovered = 0;
        hider.discovered = 0;

        initializeZerosArray(rewardsSeeker);
        initializeRewardsToBase(rewardsToBaseHider, baseX, baseY);
        initializeRewardsToBase(rewardsToBaseSeeker, baseX, baseY);
        
    }

    gameNum = 0;
    
}

gameResults Simulation::makeGame( Agent &hider, Agent &seeker) {
    gameResults gRes;

    int hiderDiscover = 0;
    double reward;
    int    baseX = seeker.getX_Coord();
    int    baseY = seeker.getY_Coord();

    for (int turn = 0; turn < turnsPerGame; turn ++) {
        turnNum++;

        if (seeker.hasWon(baseX, baseY)){
            gRes.wonBySeeker = 1;
            gRes.endTurn = turn - 1;
            turnNum = 0;
            return gRes;
        }

        if (hider.hasWon (baseX, baseY)){
            gRes.wonBySeeker = 0;
            gRes.endTurn = turn - 1;
            turnNum = 0;
            return gRes;
        }

        if (turn < hiderAdvantage){
            hider.playTurn(beta, epsilon, exploreDegree, grid);
            reward = hider.getReward(rewardsHider, turn, gRes.hiderFoundTurn, turnsPerGame);
            gRes.totalRewardHider += reward;
            hider.updateEstimates(reward, alpha, gamma, epsilon, alphaExp, beta, exploreDegree, grid);
            continue;
        }

        hider.playTurn(beta, epsilon, exploreDegree, grid);
        hiderDiscover  = seeker.playTurn(beta, epsilon, exploreDegree, grid);

        if (hiderDiscover == 1 && gRes.hiderFoundTurn == 0){
            gRes.hiderFoundTurn = turn;
            hider.discovered = 1;   // hider was discovered
            seeker.discovered = 1;  // seeker has discovered the hider
            gRes.hiderFound = 1;    // save turn when hider was found
        }

        if (gRes.hiderFoundTurn == 0 || gRes.hiderFoundTurn == turn) {
            
            reward = seeker.getReward(rewardsSeeker, turn, gRes.hiderFoundTurn, turnsPerGame);
            gRes.totalRewardSeeker += reward;
            seeker.updateEstimates(reward, alpha, gamma, epsilon, alphaExp, beta, exploreDegree, grid);

            reward = hider.getReward(rewardsHider, turn, gRes.hiderFoundTurn, turnsPerGame);
            gRes.totalRewardHider += reward;
            hider.updateEstimates(reward, alpha, gamma, epsilon, alphaExp, beta, exploreDegree, grid);

        } else {

            reward = seeker.getReward(rewardsToBaseSeeker, turn, gRes.hiderFoundTurn, turnsPerGame);
            gRes.totalRewardSeeker += reward;
            seeker.updateEstimates(reward, alpha, gamma, epsilon, alphaExp, beta, exploreDegree, grid);

            reward = hider.getReward(rewardsToBaseHider, turn, gRes.hiderFoundTurn, turnsPerGame);
            gRes.totalRewardHider += reward;
            hider.updateEstimates(reward, alpha, gamma, epsilon, alphaExp, beta, exploreDegree, grid);

        }

        // if(gameNum % 100 == 0){
        //     printSimulation(hider, seeker, gRes.hiderFound, baseX, baseY);
        // }

        // reduce seeker's reward for going back to the same spots to encourage exploration
        if (seeker.discovered == 0){
            rewardsSeeker[seeker.X*10 + seeker.Y] -= 1;
        }else if (seeker.discovered == 1 && turn > gRes.hiderFoundTurn){
            rewardsToBaseHider[hider.X*10 + hider.Y] -= 1;
            rewardsToBaseSeeker[seeker.X*10 + seeker.Y] -=1;
        }
    }

    turnNum = 0;

    gRes.endTurn = turnsPerGame;
    // IF THE WINNING CONDITION IS NOT MET, THEN : 
    // if the game ends and the hider was not found, the hider wins
    if ( hider.discovered == 0){
        gRes.wonBySeeker = 0;
    // if the game ends and the hider was found the seeker wins
    }else{
        gRes.wonBySeeker = 1;
    }


    return gRes;
}

void Simulation::printSimulation(Agent hider, Agent seeker, int hiderFound, int baseX, int baseY) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    system("clear");

    cout << "Simulation " << simNum <<"\n";

    printGrid(grid, hider, seeker, baseX, baseY);

    cout << "\nGame " << gameNum << " | Turn " << turnNum;

    if (hiderFound == 1) {
        cout << " | Hider was found!";
    }
    
    cout << "\n\n";

    hider.printCoords();
    seeker.printCoords();
}

void Simulation::printScoresPerSimulation(std::ofstream& logs, gameResults gRes) {
    logs << simNum << "," << gameNum << "," << gRes.endTurn 
         << "," << gRes.hiderFound << "," << gRes.hiderFoundTurn 
         << "," << gRes.wonBySeeker << "," 
         << gRes.totalRewardHider / gRes.endTurn << ",";
         if (gRes.endTurn - 50 > 0){
            logs << gRes.totalRewardSeeker / (gRes.endTurn-50);
         }else{
            logs << gRes.totalRewardSeeker;
         }
        
    
    logs << "\n";
}

void Simulation::createLogs(std::ofstream& logs, int hiderStrat, int seekerStrat) {
    string filename;

    filename.append("data/");

    switch (hiderStrat) {
        case 0: { filename.append("Random/Random_"); break; }
        case 1: { filename.append("Optimistic/OptimisticInit_"); break; }
        case 2: { filename.append("Epsilon/EpsilonGreedy_"); break; }
        case 3: { filename.append("Reinf/Reinforcement_"); break; }
        case 4: { filename.append("Pursuit/Pursuit_"); break; }
        case 5: { filename.append("UCB/UCB_"); break; }
    }

    filename.append("vs_");

    switch (seekerStrat) {
        case 0: { filename.append("Random"); break; }
        case 1: { filename.append("OptimisticInit"); break; }
        case 2: { filename.append("EpsilonGreedy"); break; }
        case 3: { filename.append("Reinforcement"); break; }
        case 4: { filename.append("Pursuit"); break; }
        case 5: { filename.append("UCB"); break; }
    }

    logs.open(filename + ".csv", std::ofstream::out | ios::trunc);

    logs <<"Simulation,Game,End Turn,Hider Discovered,Hider Found Turn,Seeker Won,Average Reward Hider,Average Reward Seeker\n";
}

void Simulation::printCurrentExperiment(int hiderStrat, int seekerStrat) {

    switch (hiderStrat) {
        case 0: { cout << "Random Hider"; break; }
        case 1: { cout << "Optimistic Hider"; break; }
        case 2: { cout << "Epsilon Greedy Hider"; break; }
        case 3: { cout << "Reinforcement Comparison Hider"; break; }
        case 4: { cout << "Pursuit Hider"; break; }
        case 5: { cout << "UCB Hider"; break; }
    }

    cout << " vs ";

    switch (seekerStrat) {
        case 0: { cout << "Random Seeker"; break; }
        case 1: { cout << "Optimistic Seeker"; break; }
        case 2: { cout << "Epsilon Greedy Seeker"; break; }
        case 3: { cout << "Reinforcement Comparison Seeker"; break; }
        case 4: { cout << "Pursuit Seeker"; break; }
        case 5: { cout << "UCB Seeker"; break; }
    }

    cout << "\n";
}