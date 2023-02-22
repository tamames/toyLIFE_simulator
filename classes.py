from random import uniform

import functions.func as ff

class Agent():

    def __init__(self, energy: int) -> None:
        # assert energy > 0, "Energy must be positive."
        self.genotype = ff.binary_generator()
        self.energy = energy

    def __str__(self) -> str:
        return f"Genotype: {self.genotype}. Energy: {self.energy}"

    def check_energy_reproduce(self) -> bool:
        """Checks whether the Agent has enough food to reproduce"""
        return self.energy >= 10

    def check_energy_die(self) -> bool:
        """Checks if the agent has run out of energy"""
        return self.energy < 0

    def eat(self, food: str) -> None:
        """Describe the interation between an Agent and the food.
        We make an AND operation between the two binary strings and 
        the energy gained by the Agent is the number of '1' that the
        final string has.

        Args:
            food (str): a binary string representing the food.
        """

        genotype = int(self.genotype, 2)
        food = int(food, 2)

        eaten = bin(genotype & food)  # The size of food doesn't matter

        self.energy += eaten.count('1')

class Population():
    """Representation of a population"""

    def __init__(self, size: int) -> None:
        self.generations = 0

        energies = [uniform(1,8) for _ in range(size)]  # maybe better with a normal dist
        self.population = [Agent(energy) for energy in energies]

    def __str__(self) -> str:
        return f"{[str(agent) for agent in self.population]}"
    
    def __len__(self) -> int:
        return len(self.population)

    def __getitem__(self, element: int) -> Agent:
        return self.population[element]

    def iteration(self, amount_food: int):
        """Ths should be the main loop, just chose each agent to go for food
        add a cost to the foraring process (first a cte value and then something else)
        Maybe we can do a permutation on the elements of the population to choose them randomly.
        Maybe we can control the available food to see what happends.
        """
        pass

    def after_iteration():
        """Here we check the energy of each agent and do the 
        corresponding action
        """
        pass


    def delete_elements(self, indeces: list[int]) -> None:
        """Deletes individuals from the population.
        This could happen if the individual doesn't have enough energy
        or because it has been divided

        Args:
            indeces (list[int]): the indices to remove from the population
        """
        for index in sorted(indeces, reverse=True):
            del self[index]

        
