#ifndef AGENT_H
#define AGENT_H


class Agent {
    public:
    int type = 0;  // The type of the agent: Hider(0) and Seeker(1).
    int X    = 0;  // X coordinate of the agent.
    int Y    = 0;  // Y coordinate of the agent.

    Agent(int t, std::vector<int> &grid);

    void move(int dir, std::vector<int> &grid);

    int getX_Coord();
    int getY_Coord();

    void setX_Coord(int x_coord);
    void setY_Coord(int y_coord);

    void printCoords();
};

#endif /* AGENT_H */