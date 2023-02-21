"""This module contains some functions that we are going to use in different parts"""

from random import randint


def binary_generator(amount: int = 8) -> str:
    """generates a random binary number as a str

    Args:
        amount (int, optional): The length of our binary number. Defaults to 8.

    Returns:
        str: The binary number as a str.
    """
    
    number = ""
    for _ in range(amount):
        number += str(randint(0, 1))

    return number
