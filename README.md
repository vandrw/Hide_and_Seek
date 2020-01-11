# Hide and Seek
This project creates a competitive multi-agent environment, where 2 players in each team act on a 32x32 grid.

## To Do
- [x] Implement a field of view for the seeker (and possibly the hider)
- [x] look around (explore), move (exploit)
- [x] Have the seeker and the hider spawn at a random position, with a distance higher than the vision range
- [x] if seeker finds hider, rush back to stat
- [ ] increase maze 
- [x] see first reward in front (exploit by going forward)
- [ ] hider gets the go-to-base reward from the beginning

## How to Run
To run the code, go to a terminal, set the present working directory as K-armed_Bandits and run the following commands:

```
make
make run
```

## Project Structure
* __bin__: The output executables will appear here.
* __build__: This folder contains all object files, and is removed on a `make clean`.
* __data__: The data created by the program is saved here as a csv file.
* __docs__: The graphs and other resources regarding the description go here.
* __include__: All project header files. All necessary third-party header files that do not exist under /usr/local/include are also placed here.
* __src__: The application and only the application’s source files.
