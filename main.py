from classes import Population


def main() -> None:
    # population_size = 20
    # population = [Agent(randint(1,15)) for _ in range(population_size)]
    # population = new_population(population)
    p = Population(10)
    p.after_iteration()
    print(len(p))


if __name__ == "__main__":
    main()
