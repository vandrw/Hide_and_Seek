#ifndef AGENT_H
#define AGENT_H


class Agent {
    public:
    int type = 0;  // The type of the agent: Hider(0) and Seeker(1).
    int X    = 0;  // X coordinate of the agent.
    int Y    = 0;  // Y coordinate of the agent.
    int vision = 3; // Vision range (fixed)
    int direction = 4; // Direction in which the agent previously moved and currently faces 
    std::vector<int> estimates;
    
    Agent(int t, std::vector<int> &grid);

    void act(int dir, std::vector<int> &grid);
    int check_for_wall(int direction, std::vector<int> &grid);
    int decide(double epsilon, std::vector<int> &grid);
    int find_agent_north(std::vector<int> &grid);
    int find_agent_south(std::vector<int> &grid);
    int find_agent_east(std::vector<int> &grid);
    int find_agent_west(std::vector<int> &grid);
    int find_agent(std::vector<int> &grid);
    int look_around(std::vector<int> &grid);
    int play_turn(double epsilon, std::vector<int> &grid);

    int getX_Coord();
    int getY_Coord();

    void setX_Coord(int x_coord);
    void setY_Coord(int y_coord);

    void printCoords();
};

#endif /* AGENT_H */
