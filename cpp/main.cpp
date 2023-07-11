#include "classes.cpp"
#include "functions/func.h"

int main()
{
    Population population(10);
    int generations = 100;

    int sizes[generations];

    float cost = 0.1; //the cost of the foraging process

    std::vector<std::string> foodList = listFood(100);

    for (int i = 0; i < generations; ++i)
    {
        population.iteration(foodList, foodList.size(), cost);
        population.afterIteration(0.1);
        sizes[i] = population.size_population;
    }
}