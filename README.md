## Overview
This project implements a simulator for Conway's Game of Life, a cellular automaton that evolves through generations based on simple rules. The program supports multiple tasks, including simulating generations, tracking cell changes, applying rule variations, and finding Hamiltonian paths in the resulting graphs.

## Features
- **Task 1**: Simulates K generations of the Game of Life and prints each generation.
- **Task 2**: Tracks and prints the coordinates of cells that change state between generations.
- **Task 3**: Applies two different rules (A and B) to the game and prints the resulting matrices.
- **Task 4**: Finds the longest Hamiltonian path in the graph formed by live cells.
- **Bonus (Task 5)**: Reads an initial configuration from a file and simulates the game based on predefined generations.

## Repository Structure
- `stivelib.h`: Header file containing structure definitions and function declarations.
- `stivelib.c`: Implementation of the Game of Life logic, including stack, list, tree, and graph operations.
- `master.c`: Main program that handles input/output and orchestrates the simulation based on the task number.

## Input Files
The input files for testing the program are included in the repository. These files contain:
- The task number (1-5).
- The dimensions of the matrix (N rows and M columns).
- The number of generations (K).
- The initial state of the matrix (composed of `X` for live cells and `+` for dead cells).

## Output
The program writes the results to an output file specified in the command-line arguments. The output format depends on the task:
- **Task 1**: Each generation's matrix.
- **Task 2**: Lists of changed cell coordinates for each generation.
- **Task 3**: Matrices after applying rules A and B.
- **Task 4**: The longest Hamiltonian path length and the path itself.
- **Task 5**: The final generation's matrix.

## How to Compile and Run
1. **Compile the program**:
   ```bash
   gcc master.c stivelib.c -o game_of_life
   ```

2. **Run the program**:
   ```bash
   ./game_of_life input_file output_file
   ```
   - Replace `input_file` with the path to your input file.
   - Replace `output_file` with the path to your desired output file.

## Example Usage
To simulate 5 generations of the Game of Life (Task 1):
```bash
./game_of_life data1.in output_task1.txt
```

To find the longest Hamiltonian path (Task 4):
```bash
./game_of_life data16.in output_task4.txt
```

## Notes
- The program dynamically allocates memory for matrices, lists, stacks, trees, and graphs, ensuring efficient memory usage.
- Error handling is included for file operations and memory allocation.
- The repository includes input files for testing each task.
