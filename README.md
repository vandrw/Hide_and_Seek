# Hide and Seek
This project creates a competitive multi-agent environment where 2 players in each team act on a 10x10 grid.

## How to Run
To run the code, go to a terminal, set the present working directory as Hide_and_Seek and run the following commands:

```
make run
```

In case you would like to analyze the data further, you can run `make all` to create graphs. This requires at least Python 3.7, along with the modules NumPy, Pandas, and Matplotlib. To create only the graphs, you can run `make graph`. The raw data can be found in csv files, in each strategy folder from the data folder.

If you desire to set the exploration strategy of each agent, you can run the following command:
```
make run ARGS="[0-5] [0-5]"
```
This will create an experiment where the exploration strategies of the hider and, respectively, the seeker are set to a number found in the interval 0 to 5 (including 0 and 5).

The strategies are numbered as follows:

0. Random
1. Optimistic Initial Values
2. <img src="https://render.githubusercontent.com/render/math?math=\epsilon">-Greedy
3. Reinforcement Comparison
4. Pursuit Methods
5. Upper-Confidence-Bound

To create an experiment that features a Random hider and a seeker that uses Pursuit Methods, run the following command:
```
make run ARGS="0 4"
```

Finally, there is a way to observe what the agents are doing! By running `make pretty`, you can view the grid and how the agents move in it. If you desire to choose a different set of exploration strategies, you can run the command `make run ARGS="[0-5] [0-5] print"`. This command will print every 100th game.

## Project Structure
* __bin__: The output executables will appear here.
* __build__: This folder contains all object files, and is removed on a `make clean`.
* __data__: The data created by the program is saved here as a csv file.
* __docs__: The graphs and other resources regarding the description go here.
* __include__: All project header files. All necessary third-party header files that do not exist under /usr/local/include are also placed here.
* __src__: The application and only the application’s source files.
