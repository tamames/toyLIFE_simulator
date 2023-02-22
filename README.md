# TFM

Right now I only have a class defined with a few methods.

An agent can interact with food and its energy changes. After each iteration I can generate a new population where the cells with energy >= 10 have divided. 

The whole iteration is going to be something like this:
1. generate the new population
2. Make each individual of the population find food (I don't know if they have to eat every iteration or sometimes they don't)
3. See if they have enough energy to split.
4. Repeat the process.

# Already implemented
* I have defined the main class `Agent`.
    * This class has the following atributes:
        * Genotype: a string of 0 and 1.
        * Energy: a value representing the energy of the cell.
    * And the following methods:
        * check_energy: which tells us if the cell has enough energy to reproduce.
        * check_die: tells us if the cell doesn't have energy.
* The class `Population` with:
    * Atributes
        * generation: the number of generations that have occurred.
        * population: a list with all the agents.
    * Methods
        * delete items: delete the necesarry agents in each iteration.
* A function that gives us a random binary string.
* A function that can divide a cell.
* A function to define the new population each generation.

# Things to implement:
* The main loop:
    * Make so every cell tries to find food every iteration (maybe using the permutation function to select the cells randomly).
    * Add a cost to the foraging process.
    * After each iteration check whether they have to die, reproduce or do nothing at all.
        * to check if they have to die create a boolean list with True in all the indices that have energy below 0.
        * After eleminating the dead ones see check the ones that can reproduce.
    (maybe all this can be methods??)
* The way we devide cells, we should have some information about the parent. Maybe the first one has the first half of the genotype and the second one the second half.
* Maybe the initial energy has to be normal distributed, not uniform.

* Add some representations of the population size. 