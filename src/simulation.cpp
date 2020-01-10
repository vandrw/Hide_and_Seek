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
    printGrid(grid);
    hider.printCoords();
    seeker.printCoords();
}

void makeGame(std::vector<int> grid, Agent hider, Agent seeker) {
    for (int i=0; i<1; i++) {
        makePlay(grid, hider, seeker);
    }
}

void makePlay(std::vector<int> grid, Agent hider, Agent seeker) {

    ofstream logs;
    logs.open ("logs.txt");
    
    int startX, startY;
    startX = seeker.getX_Coord();
    startY = seeker.getY_Coord();
    
    logs << "Seeker start X=" << seeker.getX_Coord() << ", Y=" << seeker.getY_Coord() << "\n";


    double epsilon = 0.01;
    int turn = 0;
    int seeker_found;
    int hider_found;
    int dir_seeker;
    int dir_hider;

    for (turn = 0; turn < 10; turn ++) {
        seeker_found = hider.play_turn(epsilon, grid);
        logs << "Turn " << turn << "\t";
        logs << "seeker_found = " << seeker_found <<"\n";

        printSimulation(grid, hider, seeker);
    }

    for (turn = 9; turn < 150; turn ++) {
        seeker_found = hider.play_turn(epsilon, grid);
        hider_found = seeker.play_turn(epsilon, grid);

        logs << "Turn " << turn << "\t";
        logs << "seeker_found = " << seeker_found <<" at location X="<< seeker.getX_Coord() << ", Y="<< seeker.getY_Coord()<<"\t";
        logs << "hider_found = "<< hider_found <<" at location X="<<hider.getX_Coord() << ", Y="<<hider.getY_Coord()<<"\n";

        printSimulation(grid, hider, seeker);
    }
    logs.close();
}