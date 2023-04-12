"""This module contains some functions that we are going to use in different parts"""

from typing import Any
import itertools
import random
import numpy as np


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


def flatten_list(l: list[list[Any]]) -> list[Any]:
    """Flattens a list into one dimension

    Args:
        l (list[list[Any]]): the list we want to flatten.

    Returns:
        list[Any]: the flattened list.
    """
    return list(itertools.chain.from_iterable(l))


def mutate(bin_string: str, p: int) -> str:
    """Given a string representing a binary number,
    it flips the bits with a probability p.

    Args:
        bin_string (str): the binary number as a string.
        p (int): the probability of flipping a bit.

    Returns:
        str: the binary string with the flipped bits.
    """
    if p < 0 or p > 1:
        raise ValueError("The probability must be between 0 and 1")

    new_string = ""
    for bit in bin_string:
        if random.random() < p:
            new_string += str(int(not int(bit)))  # flip the bit
        else:
            new_string += bit
    return new_string
