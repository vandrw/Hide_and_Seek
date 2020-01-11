// The agents will play on a grid with the following structure:

// #  0 1 2 3 4 5 6 7 8 9               #  0 1 2 3 4 5 6 7 8 9 
// 0  0 0 1 0 0 0 1 0 0 0               0      #       #      
// 1  0 0 1 0 0 0 1 0 0 1               1      #       #     # 
// 2  0 0 1 1 0 1 1 0 0 0               2      # #   # #      
// 3  0 0 0 0 0 0 0 0 0 0               3                     
// 4  1 1 1 0 0 0 0 0 0 0               4  # # #    
// 5  0 0 0 0 0 1 1 1 0 0               5            # # # 
// 6  1 1 1 1 0 1 0 0 0 0               6  # # # #   #    
// 7  0 0 0 1 0 0 0 1 1 1               7        #       # # #
// 8  0 0 0 0 0 0 0 0 1 0               8                  #
// 9  0 0 0 1 0 0 0 0 0 0               9        #         

#include <vector>
#include <iostream>
#include <random>
#include "agents.h"
#include <stdlib.h>
#include <exception>
using namespace std;

void initializeGrid(std::vector<int> &grid) {
    grid = { 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
            0, 0, 1, 1, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 0, 0, 1, 0, 0, 0, 0, 0, 0  }; 
}


void initializeRewardsGrid(std::vector<double> &rewards) {
    rewards = { 0,  1, -1,  1, 0,  1, -1,  1,  0,  1,
                0,  1, -1,  3, 0,  3, -1,  1,  1, -1,
                0,  1, -1, -1, 0, -1, -1,  1,  0,  1,
                0,  0,  2,  1, 0,  1,  1,  0,  0,  0,
                -1, -1, -1,  0, 0,  1,  1,  1,  0,  0,
                2,  2,  2,  1, 1, -1, -1, -1,  1,  0,
                -1, -1, -1, -1, 2, -1,  3,  2,  1,  1,
                1,  1,  3, -1, 0,  1,  0, -1, -1, -1,
                0,  0,  0,  0, 0,  0,  0,  3, -1,  3,
                0,  0,  1, -1, 0,  0,  0,  0,  0,  0    }; 
}


void initializeRewardsToBase(std::vector<double> &rewardsToBase, int baseX, int baseY) {
    // Rewards are equal to (18 - Manhattan distance between tile and base)/6
    // 18 is the max Manhattan distance
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            rewardsToBase[i*10+j] = (double)(18 - (abs(baseX-i) + abs(baseY-j)))/6;
            // for getting to the base first the reward is 10
            if ( i == baseX && j == baseY){
                rewardsToBase[i*10+j] = 10;
            }
        }
    }
}


void addDirection(std::vector<int> grid, std::vector<int> &grd, Agent hider, Agent seeker){
    int direction;
    for (int i=0; i<10; i++) {
        for (int j=0; j<10; j++) {
            if (grid[i*10 + j] == 2){
                direction = hider.getDirection();
                if ((direction == 0) && (i-1 >= 0)) { grd[(i-1)*10+j] = 7;}
                if ((direction == 1) && (j+1 < 10)) { grd[i*10+j+1] = 7;}
                if ((direction == 2) && (i+1 < 10)) { grd[(i+1)*10+j] = 7;}
                if ((direction == 3) && (j-1 >= 0)) { grd[i*10+j-1] = 7;}
            }
            if (grid[i*10 + j] == 3){
                direction = seeker.getDirection();
                if ((direction == 0) && (i-1 >= 0)) { grd[(i-1)*10+j] = 7;}
                if ((direction == 1) && (j+1 < 10)) { grd[i*10+j+1] = 7;}
                if ((direction == 2) && (i+1 < 10)) { grd[(i+1)*10+j] = 7;}
                if ((direction == 3) && (j-1 >= 0)) { grd[i*10+j-1] = 7;}
            }
        }
    }
}

void printGrid(std::vector<int> grid, Agent hider, Agent seeker) {

    std::vector<int> grd = grid;


    if(grd.empty()){
        cout << "Grid is empty, cannot be printed!\n";
        throw std::exception();
        return;
    }

    addDirection(grid, grd, hider, seeker);
    
    for (int i=0; i<=20; i++) { cout << "_";}

    cout << "\n";

    for (int i=0; i<10; i++) {
        cout << "|";
        for (int j=0; j<10; j++) {
            switch (grd[i*10 + j]) {
                case 0:             // Empty space           
                    cout << " ";
                    break;
                case 1:             // Wall
                    cout << "#";
                    break;
                case 2:             // Hider
                    cout << "H";
                    break;
                case 3:             // Seeker
                    cout << "S";
                    break;
                case 4:             // Goal position
                    cout << "*";
                    break;
                case 5:             // Hider at goal position
                    cout << "H";
                    break;
                case 6:             // Seeker at goal position
                    cout << "S";
                    break;
                case 7:             // Mark the direction in which the agent looks
                    cout << "+";
                    break;
            }
            if (j != 9) {cout << " ";}
        }
        cout << "|\n";
    }

    for (int i=0; i<=20; i++) { cout << "-";}
    cout << "\n";

}
