# GameOfLife
A C++ implementation of Conway's Game of Life using SFML. Randomly generates a fixed-size board of marked and unmarked cells. The cells are scanned to create the next "generation" by counting the number of neighbors in each cell.

![alt text](https://github.com/connordecamp/GameOfLife/blob/main/game_of_life.png)

## Cell Rules (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
- Any live cell with fewer than two live neighors dies
- Any live cell with two OR three live neighbors lives
- Any live cell with more than three live neighbors dies
- Any dead cell with EXACTLY three neighbors becomes a live cell

## Build Details
SFML Version 2.5.1 64bit\
Statically linked\
Compiled in Visual Studio Community 2019
