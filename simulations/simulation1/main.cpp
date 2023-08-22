#include <iostream>

#include "classes.cpp"
#include "functions/func.h"

int main() {
    Population population(100);

    float control =
        0.75f;  // The probability of a 1 appearing in the binary number.

    std::vector<std::string> listOFFood = listFood(100, control);

    std::vector<std::string> dataOfFood = statistic(listOFFood);
    writeResults(
        "food_info2",
        "The food information of the simulation, **Control = " +
            std::to_string(control) + "**",
        {"Max_1s", "Index", "Binary", "Total 1s", "Total 0s", "Total Elements"},
        {dataOfFood});

    int numberOfGenerations = 100;
    std::vector<std::vector<std::string>> dataOfGenotypes;
    dataOfGenotypes.reserve(numberOfGenerations);

    // Main loop where the simulation happens.
    for (int i = 0; i < numberOfGenerations; ++i) {
        population.iteration(listOFFood, 2.5);
        population.afterIteration(0.01);
        std::vector<std::string> genotypes = population.getGenotypes();
        std::vector<std::string> infoGenotypes = statistic(genotypes);
        infoGenotypes.push_back(std::to_string(i));
        dataOfGenotypes.push_back(infoGenotypes);
    }

    writeResults(
        "genotype_info2",
        "The information of the genotypes at each iteration, **Control = " +
            std::to_string(control) + "**",
        {"Max_1s", "Index", "Binary", "Total 1s", "Total 0s", "Total Elements",
         "Generation"},
        dataOfGenotypes);

    std::vector<std::vector<std::string>> dataOfPopulation;
    dataOfPopulation.reserve(numberOfGenerations);

    for (int i = 0; i < numberOfGenerations; ++i) {
        std::vector<std::string> dataOfGeneration;
        dataOfGeneration.reserve(4);
        dataOfGeneration.push_back(std::to_string(i));
        dataOfGeneration.push_back(
            std::to_string(population.averageEnergyGain[i]));
        dataOfGeneration.push_back(std::to_string(population.maxEnergyGain[i]));
        dataOfGeneration.push_back(std::to_string(population.minEnergyGain[i]));
        dataOfPopulation.push_back(dataOfGeneration);
    }

    writeResults("population_info2",
                 "The population information of the first simulation. "
                 "**Control = " +
                     std::to_string(control) + "**",
                 {"Generation", "Average Energy Gain", "Max Energy Gain",
                  "Min Energy Gain"},
                 dataOfPopulation);

    // std::system("pause");
}
