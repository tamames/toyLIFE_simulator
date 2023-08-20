#include <iostream>

#include "classes.cpp"
#include "functions/func.h"

int main() {
    Population population(100);

    std::vector<std::string> listOFFood = listFood(100);

    std::vector<std::string> dataOfFood = statistic(listOFFood);
    writeResults(
        "food_info1",
        "The food information of the first simulation, we have a control "
        "parameter of 0.5",
        {"Max_1s", "Index", "Binary", "Total 1s", "Total 0s", "Total Elements"},
        {dataOfFood});

    // population.iteration(listOFFood, 2.0, 0.2);

    int numberOfGenerations = 100;

    for (int i = 0; i < numberOfGenerations; ++i) {
        population.iteration(listOFFood, 2.5);
        population.afterIteration(0.01);
    }

    std::vector<std::vector<std::string>> dataOfPopulation;

    for (int i = 0; i < numberOfGenerations; ++i) {
        std::vector<std::string> dataOfGeneration;
        dataOfGeneration.push_back(std::to_string(i));
        dataOfGeneration.push_back(
            std::to_string(population.averageEnergyGain[i]));
        dataOfGeneration.push_back(std::to_string(population.maxEnergyGain[i]));
        dataOfGeneration.push_back(std::to_string(population.minEnergyGain[i]));
        dataOfPopulation.push_back(dataOfGeneration);
    }

    writeResults(
        "population_info1",
        "The population information of the first simulation, we have a control "
        "parameter of 0.5",
        {"Generation", "Average Energy Gain", "Max Energy Gain",
         "Min Energy Gain"},
        dataOfPopulation);

    // std::system("pause");
}
