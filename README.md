# GameOfLife
A C++ implementation of Conway's Game of Life using SFML. Has an "infinite" board using sparse matrices.

![conway](https://user-images.githubusercontent.com/9027769/179274522-5ed2b552-6c3f-4a66-9b68-2ed1b8a34596.gif)


## Cell Rules (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
- Any live cell with fewer than two live neighors dies
- Any live cell with two OR three live neighbors lives
- Any live cell with more than three live neighbors dies
- Any dead cell with EXACTLY three neighbors becomes a live cell

## Build Details
- SFML Version 2.5.1 32bit
- Statically linked
- Compiled in Visual Studio Community 2022

