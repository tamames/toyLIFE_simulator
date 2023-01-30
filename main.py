from random import randint

class Agent():

    def __init__(self, energy: int) -> None:
        # assert energy > 0, "Energy must be positive."
        self.genotype = binary_generator()
        self.energy = energy

    def __repr__(self) -> str:
        return f"Genotype: {self.genotype}. Energy: {self.energy}"

    def check_energy(self) -> bool:
        return self.energy >= 10
            
def divide(parent: Agent) -> tuple[Agent, Agent]:
    energy = parent.energy//2
    # print(energy)
    return Agent(energy), Agent(energy)

def binary_generator(amount: int = 8) -> str:
    """generates a random binary number as a str"""
    number = ""
    for _ in range(amount):
        number += str(randint(0,1))

    return number
        
def eat(agent: Agent, food: str) -> None:
    """This function describes how an agent interact with food"""

    genotype = int(agent.genotype, 2)
    food  = int(food, 2)

    eaten = bin(genotype & food)

    agent.energy += eaten.count('1')


def new_population(population: list[Agent]) -> list[Agent]:
    """This function must be call after all the population have eaten to see if we have to create new individuals"""
    indexis_to_remove = []
    new_individuals = []
    for i, agent in enumerate(population):
        if agent.check_energy():
            # print("alguno hay")
            indexis_to_remove.append(i)
            new_individuals.extend(divide(agent))

    if not indexis_to_remove:
        return population
    
    # remove the divided agents
    for index in sorted(indexis_to_remove, reverse=True): 
        del population[index]
    population += new_individuals
    return population

def main() -> None:
    population_size = 20
    population = [Agent(randint(1,15)) for _ in range(population_size)]
    population = new_population(population)


if __name__ == "__main__":
    main()