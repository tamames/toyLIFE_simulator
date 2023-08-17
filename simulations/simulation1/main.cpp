#include "classes.cpp"
#include "functions/func.h"

int main()
{
    Population population(100);
    std::vector<std::string> listOFFood = listFood(1000);

    int numberOfGenerations = 100;

    for (int i = 0; i < numberOfGenerations; ++i)
    {
        population.iteration(listOFFood, 100.5);
        population.afterIteration(0.01);
        std::cout << "Generation: " << i << ". Size: " << population.sizePopulation << "\n";
    }

    // std::system("pause");
}
