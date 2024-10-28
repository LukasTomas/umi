# CSP
**C**onstraint **S**atisfaction **P**roblem is defined as tuple $(X, D, C)$, where:
- $X = \\{X_1, ..., X_n\\}$ - is a set of variables
- $D = \\{D_1, ..., D_n\\}$ - is a set of nonempty domains for each variable
- $C = \\{C_1, ..., C_m\\}$ - is a set of constraints where each $C_i$ specifies allowable combinations of values for some subset of variables

## Map Coloring Problem
It's a Constraint Satisfaction Problem, where the goal is to assign one of four colours to each region of the map, so that every two neighbouring regions have different colours. 

## Implemented Algorithms

- **Backtracking** - Assigns a value to a variable from its domain and recursively continues to the next variable. If no value can be assigned without satisfying the constraints, it backtracts to the previous variable and tries to assign another value (if possible)
- **Backjumping** - Similar to backtracking, but with the difference that if the current variable cannot be assigned a value that satisfies the constraints, it can jump back over one or more previous variables. Thus reducing the size of the search space.
- **Maintained Arc Consistency with Backtracking (MAC-BT)** - After each assignment to a variable, the algorithm enforces arc consistency among neighboring variables. This means that it prunes values from their domains that would violate constraints. If the variable assignment doesn't satisfy the constraints, it backtracts to the previous variable.

# Instructions to run

1) install necessary libraries: pip install -r requirements.txt
2) run the script: ./map_coloring.py
    - use -a to set algorithm to use (BT, BJ or MAC-BT): ./map_coloring -a BT