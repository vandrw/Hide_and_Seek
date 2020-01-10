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

void makeGame(std::vector<int> grid, std::vector<double> rewards, Agent hider, Agent seeker) {
    for (int i=0; i<1; i++) {
        makePlay(grid, rewards, hider, seeker);
    }
}

void makePlay(std::vector<int> grid, std::vector<double> rewards, Agent hider, Agent seeker) {
    ofstream logs;
    logs.open ("logs.txt");
    
    int startX, startY;
    startX = seeker.getX_Coord();
    startY = seeker.getY_Coord();
    
    logs << "Seeker start X=" << seeker.getX_Coord() << ", Y=" << seeker.getY_Coord() << "\n";


    double epsilon = 0.3;
    int turn = 0;
    int seekerFound;
    int hiderFound;
    int dir_seeker;
    int dir_hider;

    for (turn = 0; turn < 50; turn ++) {
        seekerFound = hider.playTurn(epsilon, grid);
        logs << "Turn " << turn << "\t";
        logs << "seekerFound = " << seekerFound <<"\n";

        printSimulation(grid, hider, seeker);
    }

    for (turn = 49; turn < 200; turn ++) {
        seekerFound = hider.playTurn(epsilon, grid);
        hiderFound = seeker.playTurn(epsilon, grid);

        logs << "Turn " << turn << "\t";
        logs << "seekerFound = " << seekerFound <<" at location X="<< seeker.getX_Coord() << ", Y="<< seeker.getY_Coord()<<"\t";
        logs << "hiderFound = "<< hiderFound <<" at location X="<<hider.getX_Coord() << ", Y="<<hider.getY_Coord()<<"\n";

        if (hiderFound != 0){
            hider.discovered = 1;   // hider was discovered
            seeker.discovered = 1;  // seeker has discovered the hider
        }

        hider.getReward(rewards, turn, hiderFound);
        seeker.getReward(rewards, turn, hiderFound);

        printSimulation(grid, hider, seeker);
    }
    logs.close();
}