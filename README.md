# The_Cellular_Automata
This is a generator for Elementary Cellular Automata and John Conway's Game of Life.
Inspired by Stephen Wolfram's 2002 book: [A New Kind of Science](https://www.wolframscience.com/nks/).

## John Conway's Game of Life:
- The universe of the Game of Life is an infinite, two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, alive or dead.
- Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent.
- At each step in time, the following transitions occur:
  1. Any live cell with fewer than two live neighbours dies, as if by underpopulation
  2. Any live cell with two or three live neighbours lives on to the next generation
  3. Any live cell with more than three live neighbours dies, as if by overpopulation
  4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
- This creates complex patterns from very simple rules.
- There are three prederminated patterns included:
  1. [R-Pentomino](https://www.conwaylife.com/wiki/R-pentomino): A finite pattern with a predetermined lifespan.
  2. [Glider Gun](https://conwaylife.com/wiki/Gosper_glider_gun): A finite pattern with unbounded growth.
  3. [Infinite Growth](https://www.conwaylife.com/wiki/Infinite_growth): A one cell thick infinite growth pattern.

## Elementary Cellular Automata:
- Elementary Cellular Automata are the simplest class of one-dimensional cellular atomata.
- Each cell in a grid has two possible values (0 or 1) and rules that depend on their previous three nearest neighbours.
- In this program, different automata can be generated simply by changing an 8-bit binary number that represents the rules for what each cell's state should be given it's neighbours.
- More details at [Wolfram Mathworld](https://mathworld.wolfram.com/ElementaryCellularAutomaton.html).

### About the Project:
- This is my first attempt at a non-trivial project with zero influence from a tutorial. Please enjoy :)
#### Possible improvements in the future:

- Allow project to be built using Premake/CMake.
- Make project cross-platform.
- The Elementary Cellular Automata and Game of Life both use unordered maps (asssociative arrays) to store and search the cells.
Each update/access is O(log(n)), whereas if I had used an array/vector their update/access would be O(1), which would greatly improve performance. 
An ordered map (hash table) is also a viable alternative.
- Similarly, performance can be greatly improved by adding multithreaded support. 
