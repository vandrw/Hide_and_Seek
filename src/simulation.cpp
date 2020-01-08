#include <random>
#include <vector>
#include <thread>
#include <chrono>
#include "agents.h"
#include "grid.h"
#include "simulation.h"

using namespace std;

void makeGame(std::vector<int> grid, Agent hider, Agent seeker) {
    for (int i=0; i<1; i++) {
        makePlay(grid, hider, seeker);
    }
}

void makePlay(std::vector<int> grid, Agent hider, Agent seeker) {
    random_device generator;
    uniform_int_distribution<int> randInt(0, 4);

    int startX, startY;

    startX = seeker.getX_Coord();
    startY = seeker.getY_Coord();

    for (int i=0; i<10; i++) {
        hider.move(randInt(generator), grid);

        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        system("clear");
        printGrid(grid, hider, seeker);
    }

    for (int i=10; i<150; i++) {
        hider.move(randInt(generator), grid);
        seeker.move(randInt(generator), grid);

        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        system("clear");
        printGrid(grid, hider, seeker);
    }
}