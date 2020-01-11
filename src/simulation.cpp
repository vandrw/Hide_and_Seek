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

void printSimulation(std::vector<int> grid, Agent hider, Agent seeker){
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    system("clear");  
    printGrid(grid, hider, seeker);
    hider.printCoords();
    seeker.printCoords();
}

void makeGame(std::vector<int> grid, std::vector<double> rewardsHider, std::vector<double> rewardsSeeker,std::vector<double> rewardsToBase, Agent hider, Agent seeker) {
   // ofstream logs;
   // logs.open ("logs.txt");
   // logs << "Seeker start X=" << seeker.getX_Coord() << ", Y=" << seeker.getY_Coord() << "\n";

    double epsilon = 0.3;
    int seekerFound;
    int hiderFound;
    int flag = 0;
    double bonus = 0; 

    int baseX = seeker.getX_Coord();
    int baseY = seeker.getY_Coord();

    
    for (int turn = 0; turn < 200; turn ++) {
       
        if (turn < 50){
            seekerFound = hider.playTurn(epsilon, grid);
            //logs << "Turn " << turn << "\t";
            //logs << "seekerFound = " << seekerFound <<"\n";
            hider.getReward(rewardsHider, turn, bonus);
            printSimulation(grid, hider, seeker);
            continue;
        }

        seekerFound = hider.playTurn(epsilon, grid);
        hiderFound = seeker.playTurn(epsilon, grid);

       // logs << "Turn " << turn << "\t";
       // logs << "seekerFound = " << seekerFound <<" at location X="<< seeker.getX_Coord() << ", Y="<< seeker.getY_Coord()<<"\t";
       // logs << "hiderFound = "<< hiderFound <<" at location X="<<hider.getX_Coord() << ", Y="<<hider.getY_Coord()<<"\n";

        // make the bonus 10 to reward/penalize agents
        if (hiderFound != 0 && flag == 0){
            flag = 1;
            bonus = 10.0;
            hider.discovered = 1;   // hider was discovered
            seeker.discovered = 1;  // seeker has discovered the hider
        }

        if (flag == 0 || flag == 1){
            hider.getReward(rewardsHider, turn, bonus);
            seeker.getReward(rewardsSeeker, turn, bonus);
        // Once the hider is dicovered, the goal for both players becomes to get to the base 
        // (starting location of the seeker) first. So, the reward matrix changes. 
        }else{
            hider.getReward(rewardsToBase, turn, bonus);
            seeker.getReward(rewardsToBase, turn, bonus);
        }
       

        // set flag to -1 so the bonus will not become 10 if the hider is discovered again
        if (flag == 1){
            flag = -1;
            bonus = 0;
        }

        printSimulation(grid, hider, seeker);
    }
    // logs.close();
}

void makeSimulation(){
    std::vector<int> grid(100, 0);
    std::vector<double> rewardsSeeker(100, 0);
    std::vector<double> rewardsHider(100, 0);
    std::vector<double> rewardsToBase(100, 0);

    initializeGrid(grid);
    initializeRewardsGrid(rewardsHider);

    Agent  hider(0, grid);
    Agent seeker(1, grid);
    printSimulation(grid, hider, seeker);

    int baseX = hider.getX_Coord();
    int baseY = hider.getY_Coord();
    
    initializeRewardsToBase(rewardsToBase, baseX, baseY);

    for (int i = 0; i < 100; i ++ ){
        makeGame(grid, rewardsHider, rewardsSeeker, rewardsToBase, hider, seeker);
    }
    
}