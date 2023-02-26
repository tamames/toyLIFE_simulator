from random import uniform, shuffle

import functions.func as ff

import numpy as np

class Agent():

    def __init__(self, energy: float, size_genotype: int = 8, genotype: str|None = None) -> None:

        if genotype == None:
            self.genotype = ff.binary_generator()
        else:
            elements2complete = size_genotype - len(genotype)
            self.genotype = genotype + ff.binary_generator(elements2complete)
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
        """Describe the interaction between an Agent and the food.
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

    def iteration(self, amount_food: int, cost: float = 1) -> None:
        """The main loop of our population. We choose a random
        order for our agents to eat, generate an amount of food
        and sends every agent to eat. They lose energy searching for food

        Args:
            amount_food (int): the food available for each generation
            cost (float): the cost of the foraging process
        """
        order = range(len(self))
        shuffle(order) 

        foods = ff.list_food(amount_food)
        iter_food = iter(foods)

        for i in order:
            try:
                food = next(iter_food)
            except StopIteration:
                print("Not enough food for everybody")
                break
            else:
                current_agent = self[i]
                current_agent.energy -= cost
                current_agent.eat(food)

        self.generations += 1

    def after_iteration(self) -> None:
        """Here we check the energy of each agent and do the 
        corresponding action. First we check the ones that died.
        Then we check the ones that can reproduce and we reproduce them.
        """
        whole_population = np.arange(len(self))
        deads = [agent.check_energy_die() for agent in self.population] # boolean list
        self.delete_elements(whole_population[deads])

        whole_population = np.arange(len(self))
        reproduce = [agent.check_energy_reproduce()
                     for agent in self.population]  # boolean list
        new_individuals = []
        for i, agent in enumerate(self.population):
            if reproduce[i]:
                new_individuals.extend(ff.divide(agent))

        self.delete_elements(whole_population[reproduce])
        self.population += new_individuals

    def delete_elements(self, indexes: list[int]) -> None:
        """Deletes individuals from the population.
        This could happen if the individual doesn't have enough energy
        or because it has been divided

        Args:
            indeces (list[int]): the indices to remove from the population
        """
        for index in indexes[::-1]:
            del self.population[index]

        
