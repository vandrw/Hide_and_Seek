#ifndef AGENT_H
#define AGENT_H


class Agent {
    public:
    int type = 0;       // The type of the agent: Hider(0) and Seeker(1).

    int X    = 0;       // X coordinate of the agent.
    int Y    = 0;       // Y coordinate of the agent.
    int direction = 0;  // Direction in which the agent previously moved and currently faces, randomized in constructor

    int vision = 3;     // Vision range (fixed)
    int discovered = 0; // Variable showing whether the hider was found.

    std::vector<double> estimates; // Observed values = estimates of rewards in each grid tile
    

    Agent(int t, std::vector<int> &grid);

    void act(int dir, std::vector<int> &grid, double epsilon);
    int checkForWall(int direction, std::vector<int> grid);
    int decide(double epsilon, std::vector<int> grid);
    int decideRandomly(std::vector<int> grid);
    int isBlocked(std::vector<int> grid);
    int findAgentNorth(std::vector<int> grid);
    int findAgentSouth(std::vector<int> grid);
    int findAgentEast(std::vector<int> grid);
    int findAgentWest(std::vector<int> grid);
    int findAgent(std::vector<int> grid);

    int bestDirection(std::vector<int> grid);

    int playTurn(double epsilon, std::vector<int> &grid);

    double getReward(std::vector<double> rewards, int turn, int hiderFoundTurn);

    int getX_Coord();
    int getY_Coord();
    int getDirection();
    void printEstimates();

    void setX_Coord(int x_coord);
    void setY_Coord(int y_coord);

    void resizeScores();

    int hasWon(int baseX, int baseY);

    void printCoords();
};

#endif /* AGENT_H */
