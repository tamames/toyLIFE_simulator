from classes import Population
import matplotlib.pyplot as plt


def main() -> None:
    popu = Population(10)
    n_generations = 100
    sizes = [10]
    for _ in range(n_generations):
        popu.iteration(30)
        popu.after_iteration()
        sizes.append(len(popu))
    print(sizes)

    plt.plot(sizes)
    plt.show()


if __name__ == "__main__":
    main()
