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

    logs.open ("bin/logs.csv", std::ofstream::out | ios::trunc);
    logs <<"Simulation,Game,End Turn,Hider Discovered,Hider Found Turn,Seeker Won,Average Reward Hider,Average Reward Seeker\n";

    for (int i=0; i < simPerExperiment; i++) {
        simNum++;
        makeSimulation(logs);
    }

    logs.close();
}

void Simulation::makeSimulation(std::ofstream& logs) {
    gameResults gRes;

    rewardsSeeker.resize(100, 0.0);
    rewardsHider.resize(100, 0.0);
    rewardsToBase.resize(100, 0.0);

    grid.resize(100, 0);

    initializeGrid(grid);
    initializeRewardsGrid(rewardsHider);

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

        printScores(logs, gRes);

        // (Re-)set starting locations, grid & scores for next game
        initializeGrid(grid);

        hider.setX_Coord(hiderX);
        hider.setY_Coord(hiderY);

        seeker.setX_Coord(baseX);
        seeker.setY_Coord(baseY);
    }

    gameNum = 0;
    
}

gameResults Simulation::makeGame( Agent hider, Agent seeker) {
    gameResults gRes;

    int hiderDiscover = 0;

    int    flag = 0;
    double bonus = 0; 

    int    baseX = seeker.getX_Coord();
    int    baseY = seeker.getY_Coord();
    
    // Sets to -1 the turn when the hider is found

    for (int turn = 0; turn < turnsPerGame; turn ++) {
        turnNum++;

        if (seeker.hasWon(baseX, baseY)){
            gRes.wonBySeeker = 1;

            gRes.endTurn = turn - 1;

            break;
        }

        if (hider.hasWon (baseX, baseY)){
            gRes.wonBySeeker = 0;

            gRes.endTurn = turn - 1;

            break;
        }

        if (turn < hiderAdvantage){
            hider.playTurn(epsilon, grid);
            gRes.totalRewardHider += hider.getReward(rewardsHider, turn, bonus);
            // printSimulation(hider, seeker, -1);
            continue;
        }

        hider.playTurn(epsilon, grid);
        hiderDiscover  = seeker.playTurn(epsilon, grid);

        // make the bonus 10 to reward/penalize agents
        if (hiderDiscover != 0 && flag == 0){
            flag = 1;
            bonus = 10.0;
            gRes.hiderFoundTurn = turn;
            hider.discovered = 1;   // hider was discovered
            seeker.discovered = 1;  // seeker has discovered the hider

            gRes.hiderFound = 1;
        }

        if (flag == 0 || flag == 1) {

            gRes.totalRewardSeeker += seeker.getReward(rewardsSeeker, turn, bonus);

            // if in the last turn the hider was not found, he wins and gets a reward
            if (hider.discovered == 0 && turn == (turnsPerGame - 1) ){ 
                bonus = 10.0;
                gRes.wonBySeeker = 0;
            }

            gRes.totalRewardHider += hider.getReward(rewardsHider, turn, bonus);

        } else {
            // Once the hider is dicovered, the goal for both players becomes to get to the base 
            // (starting location of the seeker) first. So, the reward matrix changes. 
            gRes.totalRewardHider  +=  hider.getReward(rewardsToBase, turn, bonus);
            gRes.totalRewardSeeker += seeker.getReward(rewardsToBase, turn, bonus);
        }

        // set flag to -1 so the bonus will not become 10 if the hider is discovered again
        if (flag == 1) {
            flag = -1;
            bonus = 0;
        }

        // printSimulation(hider, seeker, gRes.hiderFound);
    }

    turnNum = 0;

    gRes.endTurn = turnsPerGame;

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

void Simulation::printScores(std::ofstream& logs, gameResults gRes) {
    logs << simNum << "," << gameNum << "," << gRes.endTurn 
         << "," << gRes.hiderFound << "," << gRes.hiderFoundTurn 
         << "," << gRes.wonBySeeker << "," 
         << gRes.totalRewardHider / turnsPerGame << ","
         << gRes.totalRewardSeeker / turnsPerGame;
    
    logs << "\n";
}