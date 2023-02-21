from random import randint

from classes import Agent, Population

            
def divide(parent: Agent) -> tuple[Agent, Agent]:
    energy = parent.energy//2
    # print(energy)
    return Agent(energy), Agent(energy)

        
def new_population(population: list[Agent]) -> list[Agent]:
    """This function must be call after all the population have eaten
     to see if we have to create new individuals
    """
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
    # population_size = 20
    # population = [Agent(randint(1,15)) for _ in range(population_size)]
    # population = new_population(population)
    p = Population(10)
    print(p[-1])


if __name__ == "__main__":
    main()