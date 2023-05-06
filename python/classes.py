from random import uniform, shuffle

import functions.func as ff

import numpy as np


class Agent:
    def __init__(
        self, energy: float, size_genotype: int = 8, genotype: str | None = None
    ) -> None:
        if genotype is None:
            self.genotype = ff.binary_generator()
        else:
            self.genotype = genotype
        self.energy = energy
        self.age = 0

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
        self.energy += eaten.count("1")


class Population:
    """Representation of a population"""

    def __init__(self, size: int) -> None:
        self.generations = 0

        energies = [
            uniform(1, 8) for _ in range(size)
        ]  # Maybe better with a lognormal dist
        self.population = [Agent(energy) for energy in energies]

    def __str__(self) -> str:
        return f"{[str(agent) for agent in self.population]}"

    def __len__(self) -> int:
        return len(self.population)

    def __getitem__(self, element: int) -> Agent:
        return self.population[element]

    def iteration(self, food: list[str], cost: float = 1) -> None:
        """The main loop of our population. We choose a random
        order for our agents to eat, generate an amount of food
        and sends every agent to eat. They lose energy searching for food

        Args:
            food (list[str]): the food available for all generations.
                The food doesn't change from one generation to another.
            cost (float): the cost of the foraging process
        """

        # Every agent searches for food
        for agent in self.population:
            agent.energy -= cost

        order = list(range(len(self)))
        shuffle(order)

        iter_food = iter(food)

        for i in order:
            try:
                food = next(iter_food)
            except StopIteration:
                # print("Not enough food for everyone")
                break
            else:
                current_agent = self[i]
                current_agent.eat(food)

        self.generations += 1
        self.add_ages()

    def after_iteration(self, p: float) -> None:
        """Here we check the energy of each agent and do the
        corresponding action. First we check the ones that died.
        Then we check the ones that can reproduce and we reproduce them.

        Args:
            p (float): the probability of mutation
        """
        whole_population = np.arange(len(self), dtype=int)
        deads = [agent.check_energy_die() for agent in self.population]  # Boolean list
        self.delete_elements(whole_population[deads])

        whole_population = np.arange(len(self), dtype=int)
        reproduce = [
            agent.check_energy_reproduce() for agent in self.population
        ]  # Boolean list
        new_individuals = [divide(self[i], p) for i in whole_population[reproduce]]
        new_individuals = ff.flatten_list(new_individuals)

        self.delete_elements(whole_population[reproduce])
        self.population += new_individuals

    def delete_elements(self, indexes: list[int]) -> None:
        """Deletes individuals from the population.
        This could happen if the individual doesn't have enough energy
        or because it has been divided

        Args:
            indexes (list[int]): the indexes to remove from the population
        """
        for index in indexes[::-1]:
            del self.population[index]

    def add_ages(self) -> None:
        """Add one to the age of every agent"""
        for agent in self.population:
            agent.age += 1


def divide(parent: Agent, p: int) -> tuple[Agent, Agent]:
    """Divide an agent that can reproduce in two. The energy of the children
    is half the energy of the parent.

    Args:
        parent (Agent): the agent that is going to be divided.
        p (int): the probability of mutation.

    Returns:
        tuple[Agent, Agent]: the two children.
    """
    energy = parent.energy // 2
    genotype_1 = ff.mutate(parent.genotype, p)
    genotype_2 = ff.mutate(parent.genotype, p)
    return Agent(energy, genotype_1), Agent(energy, genotype_2)
