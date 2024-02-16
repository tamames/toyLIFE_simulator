#include "classes.h"
#include "functions/func.h"
#include "functions/globals.h"
#include "toylife/toy_plugin.h"

int main() {
    std::cout << "Start the simulation\n";
    ToyPlugin toy;
    std::cout << "We have created the ToyPlugin" << std::endl;

    int numberOfGenerations = 500;
    int tenPercent = numberOfGenerations / 10;

    Population population(100);
    // population.print(true);

    std::cout << "In the beginning we have: " << population.sizePopulation
              << "\n";

    std::vector<std::vector<std::string>> energyInfo;
    std::vector<std::vector<std::string>> sizes = {
        {std::to_string(population.sizePopulation)}};

    std::vector<std::string> food = listOfFood(5000);

    for (int i = 1; i <= numberOfGenerations; ++i) {
        // maybe we can add the functionality of loosing energy due to the
        // foraging process
        population.iteration(food, toy);

        // we call the getEnergy function to see if someone gain energy
        // before it has a chance to devide itself
        std::vector<std::string> energies = population.getPopulationEnergy();
        energyInfo.push_back(energies);

        population.afterIteration(0.1);

        sizes.push_back({std::to_string(population.sizePopulation)});

        if (population.sizePopulation == 0) {
            std::cout << "\n All the population died in iteration: " << i
                      << std::endl;
            break;
        }
        if (i % tenPercent == 0) {
            std::cout << "Iteration: " << i
                      << ". Size: " << population.sizePopulation << std::endl;
        }
    }

    writeResults("energies",
                 "The max, average and min energy of each generation.",
                 {"Max", "Average", "Min"}, {energyInfo});

    writeResults("sizes", "The size of the population in each generation.",
                 {"Size"}, {sizes});

    // population.print(true);
}

// int main() {
//     std::vector<std::vector<std::string>> randomNumbers;

//     // Set up random number generator

//     std::uniform_int_distribution<int> dis(0, 100);

//     // Generate random numbers and store them in the 2D vector
//     for (int i = 0; i < 10; ++i) {
//         std::vector<std::string> row;
//         for (int j = 0; j < 3; ++j) {
//             int randomNumber = dis(GENERATOR);
//             row.push_back(std::to_string(randomNumber));
//         }
//         randomNumbers.push_back(row);
//     }
//     writeResults("randomNumbers", "randomNumbers.csv", {"Column1",
//     "Column2"},
//                  {randomNumbers});

//     // print the random numbers
//     for (auto it = randomNumbers.begin(); it != randomNumbers.end(); ++it) {
//         for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
//             std::cout << *it2 << "\t";
//         }
//         std::cout << "\n";
//     }
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
