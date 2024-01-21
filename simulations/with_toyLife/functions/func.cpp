#include "func.h"

#include <bitset>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <string>

#include "globals.h"

std::string binaryGenerator(int length) {
    /**
     * Generates a random binary number as a string.
     * @param amount The length of our binary number. Defaults to 8.
     * @return The binary number as a string.
     */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, 1);
    std::string binary_string = "";
    for (int i = 0; i < length; i++) {
        binary_string += std::to_string(distrib(gen));
    }
    return binary_string;
}

std::vector<float> createRandomArray(int size, int max /*= 8*/) {
    /**
     * Creates a vector and fill it with random numbers.
     * @param size The size of the array.
     * @param max The maximum value of the random integers. Defaults to 8.
     */
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dis(1, max);

    std::default_random_engine gen;
    std::uniform_real_distribution<double> distribution(0.0, max);

    std::vector<float> energies(size);
    for (int i = 0; i < size; ++i) {
        energies[i] = distribution(gen);
    }
    return energies;
}

std::string mutate(std::string binString, float p) {
    /**
     * Mutates a binary string.
     * @param binString The binary string to be mutated.
     * @param p The probability of mutation.
     * @return The mutated binary string.
     */

    if (p < 0.0f || p > 1.0f) {
        std::cout << "The probability must be between 0 and 1\n";
        throw std::invalid_argument("The probability must be between 0 and 1");
    }

    std::string newString(binString.size(), ' ');
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < binString.size(); ++i) {
        if (std::generate_canonical<double, 5>(gen) < p)
            newString[i] = binString[i] == '0' ? '1' : '0';
        else
            newString[i] = binString[i];
    }
    return newString;
}

std::vector<std::string> listOfFood(unsigned int length /*= FOOD_SIZE*/) {
    /**
     * Generates a list of random binary strings that represent the
     *  food available for a population.
     * @param length The length of the list.
     * @return The list of random binary strings.
     */

    std::vector<std::string> listOfFood(length);
    for (int i = 0; i < length; ++i)
        listOfFood[i] = binaryGenerator();

    return listOfFood;
}

std::map<std::string, int> fromList2Map(std::vector<std::string> list) {
    /**
     * Converts a list of binary strings to a map of binary strings and their
     *  respective counts.
     * @param list The list of binary strings.
     * @return The map of binary strings and their respective indexes.
     */

    std::map<std::string, int> map;
    for (int i = 0; i < list.size(); ++i)
        if (map.find(list[i]) == map.end())
            map[list[i]] = 1;
        else
            map[list[i]] += 1;

    return map;
}

std::vector<std::string> decimal2Binary(int numbers) {
    /**
     * Creates a vector with the binary representation of the numbers from 0 to
     * max_number - 1.
     * @param numbers The amount of numbers to be converted.
     * @return The vector with the binary representation of the numbers.
     */
    std::vector<std::string> binary_numbers(numbers);
    for (int i = 0; i < numbers; ++i) {
        binary_numbers[i] = std::bitset<8>(i).to_string();
    }

    return binary_numbers;
}
