#include <iostream>
#include <fstream>

#include <random>
#include <vector>
#include <thread>
#include <chrono>
#include "agents.h"
#include "grid.h"
#include "simulation.h"

using namespace std;

void Simulation::makeExperiment() {
    ofstream logs;

    logs.open ("data/logs.csv", std::ofstream::out | ios::trunc);
    logs <<"Simulation,Game,End Turn,Hider Discovered,Hider Found Turn,Seeker Won,Average Reward Hider,Average Reward Seeker\n";

    experimentResults eRes;
    adjustSize(eRes);

    for (int i=0; i < simPerExperiment; i++) {
        simNum++;
        makeSimulation(logs, eRes);
    }

    printExperimentResults(eRes);

    logs.close();
}

void Simulation::makeSimulation(std::ofstream& logs, experimentResults &eRes) {
    gameResults gRes;

    rewardsSeeker.resize(100, 0.0);
    rewardsHider.resize(100, 0.0);
    rewardsToBase.resize(100, 0.0);

    grid.resize(100, 0);

    initializeGrid(grid);
    initializeRewardsGrid(rewardsHider);
    initializeRewardsGrid(rewardsSeeker);

    Agent  hider(0, grid);
    Agent seeker(1, grid);

    int baseX  = seeker.getX_Coord();
    int baseY  = seeker.getY_Coord();
    int hiderX = hider.getX_Coord();
    int hiderY = hider.getY_Coord();

    initializeRewardsToBase(rewardsToBase, baseX, baseY);

    for (int i = 0; i < gamesPerSimulation; i ++ ) {
        gameNum++;

        gRes = makeGame(hider, seeker);

        transferGameResults(gRes, eRes, i);

        printScoresPerSimulation(logs, gRes);

        // (Re-)set starting locations, grid & scores for next game
        initializeGrid(grid);

        hider.setX_Coord(hiderX);
        hider.setY_Coord(hiderY);

        seeker.setX_Coord(baseX);
        seeker.setY_Coord(baseY);

        rewardsSeeker.resize(100, 0.0);

    }

    gameNum = 0;
    
}

gameResults Simulation::makeGame( Agent hider, Agent seeker) {
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
            return gRes;
        }

        if (hider.hasWon (baseX, baseY)){
            gRes.wonBySeeker = 0;
            gRes.endTurn = turn - 1;
            return gRes;
        }

        if (turn < hiderAdvantage){
            hider.playTurn(epsilon, grid);
            reward = hider.getReward(rewardsSeeker, turn, gRes.hiderFoundTurn);
            gRes.totalRewardHider += reward;
            hider.updateEstimates(reward, alpha, gamma, epsilon, grid);
            continue;
        }

        hider.playTurn(epsilon, grid);
        hiderDiscover  = seeker.playTurn(epsilon, grid);

        if (hiderDiscover == 1 && gRes.hiderFoundTurn == 0){
            gRes.hiderFoundTurn = turn;
            hider.discovered = 1;   // hider was discovered
            seeker.discovered = 1;  // seeker has discovered the hider
            gRes.hiderFound = 1;    // save turn when hider was found
        }

        // if (turn <= gRes.hiderFoundTurn) {
            
            reward = seeker.getReward(rewardsSeeker, turn, gRes.hiderFoundTurn);
            gRes.totalRewardSeeker += reward;
            seeker.updateEstimates(reward, alpha, gamma, epsilon, grid);

            reward = hider.getReward(rewardsSeeker, turn, gRes.hiderFoundTurn);
            gRes.totalRewardHider += reward;
            hider.updateEstimates(reward, alpha, gamma, epsilon, grid);

        // } else {

        //     gRes.totalRewardHider  +=  hider.getReward(rewardsToBase, turn, gRes.hiderFoundTurn);
        //     gRes.totalRewardSeeker += seeker.getReward(rewardsToBase, turn, gRes.hiderFoundTurn);

        // }

        // printSimulation(hider, seeker, gRes.hiderFound);

        // reduce seeker's reward for going back to the same spots to encourage exploration
        // rewardsSeeker[seeker.X*10 + seeker.Y] -= 0.5;

        // stop the game when the hider is found (until it works better)
        if (hider.discovered == 1){
            gRes.wonBySeeker = 1;
            gRes.endTurn = turn;
            return gRes;
        }

    }

    turnNum = 0;

    gRes.endTurn = turnsPerGame;
    gRes.wonBySeeker = 0;

    return gRes;
}

void Simulation::printSimulation(Agent hider, Agent seeker, int hiderFound) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    system("clear");

    cout << "Simulation " << simNum <<"\n";

    printGrid(grid, hider, seeker);

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
         << gRes.totalRewardHider / turnsPerGame << ","
         << gRes.totalRewardSeeker / turnsPerGame;
    
    logs << "\n";
}

void Simulation::adjustSize(experimentResults &eRes){
    eRes.endTurns.resize(gamesPerSimulation,0.0);
    eRes.hiderFoundTurn.resize(gamesPerSimulation,0.0);
    eRes.hiderFound.resize(gamesPerSimulation,0.0);
    eRes.hiderRewards.resize(gamesPerSimulation,0.0);
    eRes.seekerRewards.resize(gamesPerSimulation,0.0);
    eRes.seekerWins.resize(gamesPerSimulation,0.0);
}

void Simulation::transferGameResults(gameResults gRes, experimentResults &eRes, int i){
    eRes.endTurns[i] += gRes.endTurn;
    eRes.hiderFound[i] += gRes.hiderFound;
    if(gRes.hiderFoundTurn != -1){ 
        eRes.hiderFoundTurn[i] += gRes.hiderFoundTurn;
    }else {
        eRes.hiderFoundTurn[i] += turnsPerGame;
    }
    eRes.hiderRewards[i] += gRes.totalRewardHider;
    eRes.seekerRewards[i] += gRes.totalRewardSeeker;
    eRes.seekerWins[i] += gRes.wonBySeeker;
}

void Simulation::printExperimentResults(experimentResults eRes){
    ofstream experiment;
    experiment.open("data/experiment.csv", std::ofstream::out | ios::trunc);
    experiment <<"Game,End Turn,Hider Discovered,Hider Found Turn,Seeker Won,Average Reward Hider,Average Reward Seeker\n";
    
    for (int i = 0; i<gamesPerSimulation; i++){
        experiment  << i << "," << eRes.endTurns[i]/simPerExperiment << "," 
                    << eRes.hiderFound[i]/simPerExperiment << "," 
                    << eRes.hiderFoundTurn[i]/simPerExperiment << ","
                    << eRes.seekerWins[i]/simPerExperiment << ","
                    << eRes.hiderRewards[i]/(simPerExperiment*eRes.endTurns[i]) << ","
                    << eRes.seekerRewards[i]/(simPerExperiment*eRes.endTurns[i]) <<"\n";              
    }

    experiment.close();
}