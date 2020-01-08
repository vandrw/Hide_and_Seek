// The agents will play on a grid with the following structure:

// #  1 2 3 4 5 6 7 8 9 10              #  1 2 3 4 5 6 7 8 9 10
// 1  0 0 1 0 0 0 1 0 0 0               1      #       #      
// 2  0 0 1 0 0 0 1 0 0 1               2      #       #     # 
// 3  0 0 1 1 0 1 1 0 0 0               3      # #   # #      
// 4  0 0 0 0 0 0 0 0 0 0               4                     
// 5  1 1 1 0 0 0 0 0 0 0               5  # # #    
// 6  0 0 0 0 0 1 1 1 0 0               6            # # # 
// 7  1 1 1 1 0 1 0 0 0 0               7  # # # #   #    
// 8  0 0 0 1 0 0 0 1 1 1               8        #       # # #
// 9  0 0 0 0 0 0 0 0 1 0               9                  #
// 10 0 0 0 1 0 0 0 0 0 0               10       #         

#include <vector>
#include <iostream>
#include <random>
#include "agents.h"

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

void printGrid(std::vector<int> grid, Agent hider, Agent seeker) {
    for (int i=0; i<=20; i++) { cout << "_";}

    cout << "\n";

    for (int i=0; i<10; i++) {
        cout << "|";

        for (int j=0; j<10; j++) {

            switch (grid[i*10 + j]) {
                case 0:
                    cout << " ";
                    break;
                case 1:
                    cout << "#";
                    break;
                case 2:
                    cout << "H";
                    break;
                case 3:
                    cout << "S";
                    break;
                case 4:
                    cout << "*";
                    break;
                case 5:
                    cout << "H";
                    break;
                case 6:
                    cout << "S";
                    break;
            }
            if (j != 9) {cout << " ";}
        }
        cout << "|\n";
    }

    for (int i=0; i<=20; i++) { cout << "-";}
    cout << "\n";

    hider.printCoords();
    seeker.printCoords();
}
