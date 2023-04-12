from classes import Population
import functions.func as ff

import matplotlib.pyplot as plt


def main() -> None:
    popu = Population(50)
    n_generations = 100
    food = ff.list_food(100)
    sizes = [10]
    for _ in range(n_generations):
        popu.iteration(food)
        popu.after_iteration(0.3)
        sizes.append(len(popu))
    print(sizes)

    plt.plot(sizes)
    plt.show()


if __name__ == "__main__":
    main()
