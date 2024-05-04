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
    int initialPopulationSize = INITIAL_POPULATION_SIZE;
    int foodSize = FOOD_SIZE;

    createReadMe(numberOfGenerations, initialPopulationSize, foodSize);

    Population population(initialPopulationSize);
    // population.print(true);

    std::cout << "In the beginning we have: " << population.sizePopulation
              << "\n";

    std::vector<std::vector<std::string>> energyInfo;
    std::vector<std::vector<std::string>> energyGains;
    std::vector<std::vector<std::string>> sizes = {
        {std::to_string(population.sizePopulation)}};

    // we write the first step of the simulation
    std::vector<std::vector<std::string>> dataOfSimulation =
        population.getPopulationData();
    populationWriting(dataOfSimulation, 0, dataDirectory);

    std::vector<std::string> food = listOfFood(foodSize);

    foodWriting(food, dataDirectory);

    for (int i = 1; i <= numberOfGenerations; ++i) {
        // maybe we can add the functionality of loosing energy due to the
        // foraging process
        population.iteration(food, toy);

        // we call the getEnergy function to see if someone gain energy
        // before it has a chance to divide itself
        std::vector<std::string> energies = population.getPopulationEnergy();
        energyInfo.push_back(energies);

        population.afterIteration(0.1);

        sizes.push_back({std::to_string(population.sizePopulation)});

        if (population.sizePopulation == 0) {
            std::cout << "\n All the population died in iteration: " << i
                      << std::endl;
            break;
        }

        // we write only if the population is not dead
        std::vector<std::vector<std::string>> dataOfSimulation =
            population.getPopulationData();
        populationWriting(dataOfSimulation, i, dataDirectory);

        if (i % tenPercent == 0) {
            std::cout << "Iteration: " << i
                      << ". Size: " << population.sizePopulation << std::endl;
        }
    }

    std::vector<std::vector<std::string>> gainedEnergies =
        population.getPopulationGains();

    writeResults("energy_gains", dataDirectory,
                 {"Max_Gain", "Average_Gain", "Total_Gain", "Min_Gain"},
                 gainedEnergies);

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

//     int initialPopulationSize = INITIAL_POPULATION_SIZE;
//     int foodSize = FOOD_SIZE;
//     std::cout << "Nos disponemos a crear la población\n";
//     Population population(10);
//     std::cout << "Hemos creado la población\n";
//     // population.print(true);
//     std::cout << "Vamos a crear la comida\n";
//     std::vector<std::string> food = listOfFood(foodSize);
//     std::cout << "Hemos creado la comida\n";

//     population.iteration(food, toy);
//     population.afterIteration(0.1);

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
