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
    * This class has the following attributes:
        * Genotype: a string of 0 and 1. If the cell has a parent the first half of the genotype is the genotype of the parent.
        * Energy: a value representing the energy of the cell.
    * And the following methods:
        * check_energy: which tells us if the cell has enough energy to reproduce.
        * check_die: tells us if the cell doesn't have energy.
* The class `Population` with:
    * Attributes
        * generation: the number of generations that have occurred.
        * population: a list with all the agents.
    * Methods
        * delete items: delete the necessary agents in each iteration.
        * iteration: the main loop of our cells.
        * after_iteration: First we check the dead ones and then the ones that can reproduce.
* A function that gives us a random binary string.
* A function that can divide a cell.
* A function to define the new population each generation.

# Things to implement:
* The main loop:
    * After each iteration check whether they have to die, reproduce or do nothing at all.
        * to check if they have to die create a boolean list with True in all the indices that have energy below 0.
        * After eliminating the dead ones see check the ones that can reproduce.
    (maybe all this can be methods??)
* Maybe the initial energy has to be normal distributed, not uniform.

* Add some representations of the population size. 