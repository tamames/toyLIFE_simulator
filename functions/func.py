"""This module contains some functions that we are going to use in different parts"""

import numpy as np

from classes import Agent


def binary_generator(length: int = 8) -> str:
    """generates a random binary number as a str

    Args:
        amount (int, optional): The length of our binary number. Defaults to 8.

    Returns:
        str: The binary number as a str.
    """
    number = np.random.randint(0, 2, length).astype("str")
    return "".join(number)


def list_food(amount: int) -> list[str]:
    """Generates the food available for each iteration.
    The food is represented as a binary string

    Args:
        amount (int): the amount of food that is available.

    Returns:
        list[str]: a list containing the food.
    """
    return [binary_generator() for _ in range(amount)]


def divide(parent: Agent) -> tuple[Agent, Agent]:
    """Divide an agent that can reproduce in two. The energy of the childs
    is half the energy of the parent.

    Args:
        parent (Agent): the agent that is going to be divided.

    Returns:
        tuple[Agent, Agent]: the two childs.
    """
    energy = parent.energy // 2
    size_genotype = int(len(parent.genotype) // 2)
    genotype_1, genotype_2 = (
        parent.genotype[:size_genotype],
        parent.genotype[size_genotype:],
    )
    return Agent(energy, genotype_1), Agent(energy, genotype_2)
