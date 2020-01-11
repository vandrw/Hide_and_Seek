#include <random>
#include <vector>
#include <thread>
#include <chrono>
#include "agents.h"
#include "grid.h"
#include "simulation.h"

#include <iostream>
#include <fstream>

using namespace std;

void printSimulation(std::vector<int> grid, Agent hider, Agent seeker, int turn){
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    system("clear");  
    printGrid(grid, hider, seeker);
    cout <<"Turn " << turn << "\n";
    hider.printCoords();
    seeker.printCoords();
}

int makeGame(int turnsPerGame, std::vector<double> &hiderScores, std::vector<double> &seekerScores ,std::vector<int> &grid, std::vector<double> rewardsHider, std::vector<double> rewardsSeeker,std::vector<double> rewardsToBase, Agent hider, Agent seeker) {
   // hyper-parameters
    double epsilon = 0.5;
    int hiderAdvantage = 50;

    int seekerFound;
    int hiderFound;
    int flag = 0;
    double bonus = 0; 

    int baseX = seeker.getX_Coord();
    int baseY = seeker.getY_Coord();
    
    // Sets to -1 the turn when the hider is found
    hiderScores[1] = -1;
    seekerScores[1] = -1;

    for (int turn = 0; turn < turnsPerGame; turn ++) {
        if (seeker.hasWon(baseX, baseY)){
            seekerScores[0] = 1;
            hiderScores[0] = 1;
            return turn-1;
        }
        if (hider.hasWon (baseX, baseY)){
            seekerScores[0] = 0;
            hiderScores[0] = 0;
            return turn-1;
        }
        if (turn < hiderAdvantage){
            seekerFound = hider.playTurn(epsilon, grid);
            hiderScores[turn+2] = hider.getReward(rewardsHider, turn, bonus);
            printSimulation(grid, hider, seeker, turn);
            continue;
        }

        seekerFound = hider.playTurn(epsilon, grid);
        hiderFound = seeker.playTurn(epsilon, grid);

        // make the bonus 10 to reward/penalize agents
        if (hiderFound != 0 && flag == 0){
            flag = 1;
            bonus = 10.0;
            hiderScores[1] = turn;
            seekerScores[1] = turn;
            hider.discovered = 1;   // hider was discovered
            seeker.discovered = 1;  // seeker has discovered the hider
        }

        if (flag == 0 || flag == 1){
            seekerScores[turn+2] = seeker.getReward(rewardsSeeker, turn, bonus);
            // if in the last turn the hider was not found, he wins and gets a reward
            if (hider.discovered == 0 && turn == 199){ 
                bonus = 10;
                hiderScores[0] = 0;
                seekerScores[0] = 0;
            }
            hiderScores[turn+2] = hider.getReward(rewardsHider, turn, bonus);
        // Once the hider is dicovered, the goal for both players becomes to get to the base 
        // (starting location of the seeker) first. So, the reward matrix changes. 
        }else{
            hiderScores[turn+2] = hider.getReward(rewardsToBase, turn, bonus);
            seekerScores[turn+2] = seeker.getReward(rewardsToBase, turn, bonus);
        }       
        // set flag to -1 so the bonus will not become 10 if the hider is discovered again
        if (flag == 1){
            flag = -1;
            bonus = 0;
        }

        printSimulation(grid, hider, seeker, turn);
    }

    return turnsPerGame;
}


void printScores(std::ofstream& logs, int game, int endTurn, std::vector<double> hiderScores, std::vector<double> seekerScores){
    logs << game << ",hider,"<< endTurn << ",";
    if (hiderScores[0] == 0){ logs <<"hider,";
    }else{ logs <<"seeker,";}
    for (int i = 1; i < endTurn + 2; i++){
        logs << hiderScores[i] <<" ";
    }
    logs << "\n" << game << ",seeker," << endTurn << ",";

    if (seekerScores[0] == 0){ logs <<"hider,";
    }else{ logs <<"seeker,"; }

    for (int i = 1; i < endTurn + 2; i++){
        logs << seekerScores[i] <<" ";
    }
    logs <<"\n";
}

void makeSimulation(int turnsPerGame, int gamesPerSimulation, std::ofstream& logs){
    std::vector<int> grid(100, 0);
    std::vector<double> rewardsSeeker(100, 0);
    std::vector<double> rewardsHider(100, 0);
    std::vector<double> rewardsToBase(100, 0);

    initializeGrid(grid);
    initializeRewardsGrid(rewardsHider);

    Agent  hider(0, grid);
    Agent seeker(1, grid);
    printSimulation(grid, hider, seeker, -1);

    int baseX = seeker.getX_Coord();
    int baseY = seeker.getY_Coord();
    int hiderX = hider.getX_Coord();
    int hiderY = hider.getY_Coord();

    initializeRewardsToBase(rewardsToBase, baseX, baseY);

    std::vector<double> hiderScores(turnsPerGame+2, 0);
    std::vector<double> seekerScores(turnsPerGame+2, 0);
    int endTurn;

    for (int i = 0; i < gamesPerSimulation; i ++ ){
        endTurn = makeGame(turnsPerGame, hiderScores, seekerScores, grid, rewardsHider, rewardsSeeker, rewardsToBase, hider, seeker);
        printScores(logs, i, endTurn, hiderScores, seekerScores);
        // (re-)set starting locations, grid & scores for next game
        initializeGrid(grid);
        hider.setX_Coord(hiderX);
        hider.setY_Coord(hiderY);
        seeker.setX_Coord(baseX);
        seeker.setY_Coord(baseY);
        hiderScores.resize(turnsPerGame+2, 0);
        seekerScores.resize(turnsPerGame+2, 0);
    }
    
}