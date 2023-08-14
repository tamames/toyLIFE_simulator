#pragma once

#include <random>
#include <string>
#include <stdexcept>
#include <algorithm> // std::count std::max_element
#include <numeric>   // std::accumulate

typedef std::vector<std::string> foodList;

std::string binaryGenerator(int length = 8)
{
    /**
     * Generates a random binary number as a string.
     * @param amount The length of our binary number. Defaults to 8.
     * @return The binary number as a string.
     */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, 1);
    std::string binary_string = "";
    for (int i = 0; i < length; i++)
    {
        binary_string += std::to_string(distrib(gen));
    }
    return binary_string;
}

std::vector<float> createRandomArray(int size, int max = 8)
{
    /**
     * Creates a vector and fill it with random numbers.
     * @param size The size of the array.
     * @param max The maximum value of the random integers. Defaults to 8.
     */
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dis(1, max);

    std::default_random_engine gen;
    std::uniform_real_distribution<double> distribution(0.0,
                                                        max);

    std::vector<float> energies(size);
    for (int i = 0; i < size; ++i)
    {
        energies[i] = distribution(gen);
    }
    return energies;
}

std::string mutate(std::string binString, float p)
{
    /**
     * Mutates a binary string.
     * @param binString The binary string to be mutated.
     * @param p The probability of mutation.
     * @return The mutated binary string.
     */

    if (p < 0.0f || p > 1.0f)
    {
        std::cout << "The probability must be between 0 and 1\n";
        throw std::invalid_argument("The probability must be between 0 and 1");
    }

    std::string newString(binString.size(), ' ');
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < binString.size(); ++i)
    {
        if (std::generate_canonical<double, 5>(gen) < p)
            newString[i] = binString[i] == '0' ? '1' : '0';
        else
            newString[i] = binString[i];
    }
    return newString;
}

foodList listFood(unsigned int length)
{
    /**
     * Generates a list of random binary strings that represent the
     *  food available for a population.
     * @param length The length of the list.
     * @return The list of random binary strings.
     */

    foodList listOFFood(length);
    for (int i = 0; i < length; ++i)
        listOFFood[i] = binaryGenerator();

    return listOFFood;
}

void statistic(foodList listOFFood)
{
    /**
     * Prints the statistics of the food list.
     * Such as the percentage of 1s in total and the
     * maximum and minimum number of 1s in a binary string.
     * @param listOFFood The list of food.
     */

    int totalSize = listOFFood.size();
    int eachSize = listOFFood[0].size();
    std::vector<int> ones(totalSize, 0);
    std::vector<int> zeros(totalSize, 0);
    for (int i = 0; i < totalSize; ++i)
    {

        ones[i] = std::count(listOFFood[i].begin(), listOFFood[i].end(), '1');
        zeros[i] = eachSize - ones[i];
    }

    int totalOnes = std::accumulate(ones.begin(), ones.end(), 0);
    int totalZeros = std::accumulate(zeros.begin(), zeros.end(), 0);

    auto maxOne = std::max_element(ones.begin(), ones.end());
    int maxOneValue = *maxOne;
    int maxOneIndex = std::distance(ones.begin(), maxOne);

    std::cout << "The maximum number of 1s in a binary string is " << maxOneValue << "\n";
    std::cout << "And it's obtain in the index " << maxOneIndex << ": " << listOFFood[maxOneIndex] << "\n";
    std::cout << "The number of 1s in the food list is " << totalOnes << "\n";
    std::cout << "The number of 0s in the food list is " << totalZeros << "\n";
    std::cout << "The number of elements is " << totalSize * eachSize << "\n";
    std::cout << totalOnes + totalZeros << "\n";

    // std::cout << "The food list has " << totalSize << " elements.\n";
    // std::cout << "The food list has " << ones << " ones.\n";
    // std::cout << "The food list has " << zeros << " zeros.\n";
}