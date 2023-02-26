from random import randint

import numpy as np

from classes import Agent, Population

def main() -> None:
    # population_size = 20
    # population = [Agent(randint(1,15)) for _ in range(population_size)]
    # population = new_population(population)
    p = Population(10)
    de = p.after_iteration()
    


if __name__ == "__main__":
    main()
    