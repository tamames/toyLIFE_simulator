#include <ctime>

#include "classes.h"
#include "functions/func.h"
#include "functions/globals.h"
#include "toylife/toy_plugin.h"

int main() {
    std::cout << "Start the simulation\n";
    ToyPlugin toy;
    std::cout << "We have created the ToyPlugin" << std::endl;

    std::cout << "Creating the data directory\n";
    std::string dataDirectory = createDataDirectory();

    int numberOfGenerations = NUMBER_OF_GENERATIONS;
    int tenPercent = numberOfGenerations / 10;
    int fivePercent = numberOfGenerations / 20;
    int initialPopulationSize = INITIAL_POPULATION_SIZE;
    int foodSize = INITIAL_FOOD_SIZE;

    // to prevent errors
    tenPercent = tenPercent == 0 ? 1 : tenPercent;
    fivePercent = fivePercent == 0 ? 1 : fivePercent;

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
    foodWriting(food, dataDirectory, 0);

    std::map<std::string, int> food2eatMap = fromList2Map(food);

    // to free some memory
    food.clear();

    for (int i = 1; i <= numberOfGenerations; ++i) {
        bool print = ((i % fivePercent == 0) && !(i % tenPercent == 0));

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

        addKeysToFoodMap(food2eatMap, returnedFood, newFood);

        foodWriting(returnedFood, dataDirectory, i);

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

// int main() {
//     std::cout << "Start the simulation\n";
//     ToyPlugin toy;
//     std::cout << "We have created the ToyPlugin" << std::endl;

//     std::vector<std::string> food = {
//         "00010000", "00101000", "00101010", "01100101", "10010110",
//         "10010111", "10100010", "11001110", "11100001", "11101010",
//     };

//     std::map<std::string, int> foodMap = fromList2Map(food);
//     food.clear();

//     // print the food map
//     std::cout << "The map: \n";
//     for (auto food : foodMap) {
//         std::cout << food.first << "\t" << food.second << std::endl;
//     }

//     Agent a(10);

//     std::map<std::string, int> ja = sampleFood(foodMap);

//     std::cout << "The ja: \n";
//     for (auto food : ja) {
//         std::cout << food.first << "\t" << food.second << std::endl;
//     }

//     a.eat(ja, toy);

//     std::cout << "The ja: \n";
//     for (auto food : ja) {
//         std::cout << food.first << "\t" << food.second << std::endl;
//     }

//     return 0;
// }

// int main(){
//     std::cout << "Hello world!" << std::endl;

//     int a = 10;
//     int b = 20;
//     std::cout << "a: " << a << "\tb: " << b << std::endl;
// }

// int main() {
//     std::vector<std::string> food = listOfFood(10);
//     // print the food vector
//     std::cout << "The vector: \n";
//     for (auto it = food.begin(); it != food.end(); ++it) {
//         std::cout << *it << "\n";
//     }

//     std::vector<std::string> out = sampleFood(food);
//     // std::sample(food.begin(), food.end(), std::back_inserter(out), 5,
//     //             GENERATOR);

//     std::cout << "The sample: \n";
//     for (auto it = out.begin(); it != out.end(); ++it) {
//         std::cout << *it << "\n";
//     }

//     // std::vector<std::string> out2;
//     // std::sample(food.begin(), food.end(), std::back_inserter(out2), 5,
//     //             GENERATOR);

//     // std::cout << "The sample2: \n";
//     // for (auto it = out2.begin(); it != out2.end(); ++it) {
//     //     std::cout << *it << "\n";
//     // }
// }

// this is an example with genotypes that breaks food
// int main() {
//     std::cout << "Hello world!" << std::endl;

//     ToyPlugin toy;
//     std::cout << "Se crea el toyPlugin" << std::endl;

//     std::string gen =
//         "111101010001110110101100100100100000101110111010101101011011";

//     gen = "111101010001110110101011101010110101101111001001001000001011";

//     std::cout << bintodec("11000101") << std::endl;

//     std::vector<std::string> food = decimal2Binary(256);
//     for (auto it = food.begin(); it != food.end(); ++it) {
//         std::cout << *it << "\t" << bintodec(*it) << std::endl;
//         Agent test(100.0, gen);
//         std::map<std::string, int> foodMap;
//         foodMap[*it] = 1;
//         bool a = test.metabolism(foodMap, toy);
//         if (a)
//             std::cout << "Breaks " << *it << "\t" << bintodec(*it) <<
//             std::endl;
//     }

//     std::cout << "End\n" << std::endl;

//     int intFood = 47;

//     // Examples of food that can be broken by the Agent 00010011,
//     00010111
// }
