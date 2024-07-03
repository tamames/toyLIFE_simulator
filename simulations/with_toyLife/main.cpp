#include <time.h>

#include "classes.h"
#include "functions/func.h"
#include "functions/globals.h"
#include "toylife/toy_plugin.h"

int main() {
    std::cout << "Start the simulation\n";
    ToyPlugin toy;
    std::cout << "We have created the ToyPlugin" << std::endl;

    std::cout << "Creating the data directory\n";
    std::filesystem::path dataDirectory = createDataDirectory();

    int numberOfGenerations = NUMBER_OF_GENERATIONS;
    int tenPercent = numberOfGenerations / 10;
    int percentToPrint = numberOfGenerations / 4;
    int initialPopulationSize = INITIAL_POPULATION_SIZE;
    int foodSize = INITIAL_FOOD_SIZE;

    // to prevent errors
    tenPercent = tenPercent == 0 ? 1 : tenPercent;
    percentToPrint = percentToPrint == 0 ? 1 : percentToPrint;

    createReadMe(numberOfGenerations, initialPopulationSize, foodSize);

    Population population(initialPopulationSize);
    // population.print(true);

    std::cout << "In the beginning we have: " << population.sizePopulation
              << "\n";

    std::vector<std::vector<std::string>> energyInfo;
    energyInfo.reserve(numberOfGenerations);

    std::vector<std::vector<std::string>> sizes;
    sizes.reserve(numberOfGenerations);
    sizes.push_back({std::to_string(population.sizePopulation)});

    std::cout << "Writing the first step of the simulation\n";
    // we write the first step of the simulation
    std::vector<std::vector<std::string>> dataOfSimulation =
        population.getPopulationData();
    populationWriting(dataOfSimulation, 0, dataDirectory);

    std::vector<std::string> food = listOfFood(foodSize);
    std::cout << "Writing the first food\n";
    foodWriting(food, dataDirectory, 0, 'O');

    std::map<std::string, int> food2eatMap = fromList2Map(food);

    // to free some memory
    food.clear();

    double startTime, finishTime;

    // main loop
    for (int i = 1; i <= numberOfGenerations; ++i) {
        startTime = (double)clock();

        bool print = ((i % percentToPrint == 0) && !(i % tenPercent == 0));

        std::vector<std::map<std::string, int>> returnedFood =
            population.iteration(food2eatMap, toy, print, i);

        if (print) {
            std::cout << currentTime() << "   getting energy" << std::endl;
        }
        // we call the getEnergy function to see if someone gain energy
        // before it has a chance to divide itself
        std::vector<std::string> energies = population.getPopulationEnergy();
        energyInfo.push_back(energies);
        if (print) {
            std::cout << currentTime() << "   after getEnergy" << std::endl;
        }

        population.afterIteration(print);

        sizes.push_back({std::to_string(population.sizePopulation)});

        if (population.sizePopulation == 0) {
            std::cout << "\n All the population died in iteration: " << i
                      << std::endl;
            break;
        }

        if (print) {
            std::cout << currentTime() << "   writing data" << std::endl;
        }
        // we write only if the population is not dead
        std::vector<std::vector<std::string>> dataOfSimulation =
            population.getPopulationData();
        populationWriting(dataOfSimulation, i, dataDirectory);

        if (print) {
            std::cout << currentTime() << "   writing food" << std::endl;
        }

        // Now we deal with the new food that we have to add to the population
        std::vector<std::string> newFood = listOfFood(FOOD_TO_ADD);

        // first we write the food. The new one and the returned one.
        foodWriting(returnedFood, dataDirectory, i, 'R');
        foodWriting(newFood, dataDirectory, i, 'N');
        foodWriting({food2eatMap}, dataDirectory, i, 'P');

        // then we add the new food to the food2eatMap
        addKeysToFoodMap(food2eatMap, returnedFood, newFood);

        if (print) {
            std::cout << currentTime() << "   after writing food" << std::endl;
        }

        if (i % tenPercent == 0) {
            std::cout << currentTime() << ":   Iteration: " << i
                      << ". Size: " << population.sizePopulation << std::endl;
        }

        if (population.sizePopulation > MAXIMUM_POPULATION_SIZE) {
            std::cout << "The population has exceeded the limit of "
                      << MAXIMUM_POPULATION_SIZE << " in iteration: " << i
                      << std::endl;
            break;
        }

        finishTime = (double)clock();
        std::cout << "Time of iteration " << i << ": "
                  << (finishTime - startTime) / CLOCKS_PER_SEC
                  << ". Size: " << population.sizePopulation << std::endl;
    }

    std::cout << currentTime() << "    End of the simulation\n" << std::endl;

    std::vector<std::vector<std::string>> gainedEnergies =
        population.getPopulationGains();

    writeResults("energy_gains", dataDirectory,
                 {"Max_Gain", "Average_Gain", "Total_Gain", "Min_Gain"},
                 gainedEnergies);

    std::vector<std::vector<std::string>> deadsReproduce =
        population.getDeadsAndReproduces();

    writeResults("deads_reproduce", dataDirectory, {"Deads", "Reproduce"},
                 deadsReproduce);

    writeResults("energies", dataDirectory, {"Max", "Average", "Min"},
                 {energyInfo});

    writeResults("sizes", dataDirectory, {"Size"}, {sizes});

    increaseNumberOfSimulation();
    return 0;
}
