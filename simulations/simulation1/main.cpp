#include <iostream>

#include "classes.cpp"
#include "functions/func.h"
#include "functions/globals.cpp"

int main() {
    Population population(100);

    std::string fileName = "food_stats";
    std::vector<std::string> listOFFood = listFood(100, CONTROL);
    foodWriting(listOFFood);

    // std::vector<std::string> dataOfFood = statistic(listOFFood);
    // writeResults(
    //     fileName, "The food statistics",
    //     {"Max_1s", "Index", "Binary", "Total 1s", "Total 0s", "Total
    //     Elements"}, {dataOfFood});

    // int numberOfGenerations = 100;
    // std::vector<std::vector<std::string>> dataOfGenotypes;
    // dataOfGenotypes.reserve(numberOfGenerations);

    // // Main loop where the simulation happens.
    // for (int i = 0; i < numberOfGenerations; ++i) {
    //     population.iteration(listOFFood, 2.5);
    //     population.afterIteration(0.01);
    //     std::vector<std::string> genotypes = population.getGenotypes();
    //     std::vector<std::string> infoGenotypes = statistic(genotypes);
    //     infoGenotypes.push_back(std::to_string(i));
    //     dataOfGenotypes.push_back(infoGenotypes);
    // }

    // fileName = "genotype_info";
    // writeResults(fileName,
    //              "The information of the genotypes at each iteration.",
    //              {"Max_1s", "Index", "Binary", "Total 1s", "Total 0s",
    //               "Total Elements", "Generation"},
    //              dataOfGenotypes);

    // std::vector<std::vector<std::string>> dataOfPopulation;
    // dataOfPopulation.reserve(numberOfGenerations);

    // for (int i = 0; i < numberOfGenerations; ++i) {
    //     std::vector<std::string> dataOfGeneration;
    //     dataOfGeneration.reserve(4);
    //     dataOfGeneration.push_back(std::to_string(i));
    //     dataOfGeneration.push_back(
    //         std::to_string(population.averageEnergyGain[i]));
    //     dataOfGeneration.push_back(std::to_string(population.maxEnergyGain[i]));
    //     dataOfGeneration.push_back(std::to_string(population.minEnergyGain[i]));
    //     dataOfPopulation.push_back(dataOfGeneration);
    // }

    // fileName = "population_info";
    // writeResults(fileName, "The population information of the simulation. ",
    //              {"Generation", "Average Energy Gain", "Max Energy Gain",
    //               "Min Energy Gain"},
    //              dataOfPopulation);

    // std::system("pause");
}
