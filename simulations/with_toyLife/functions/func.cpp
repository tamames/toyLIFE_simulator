#include "func.h"

#include <bitset>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "../toylife/toy_plugin.h"
#include "globals.h"

std::string binaryGenerator(int length) {
    /**
     * Generates a random binary number as a string.
     * @param amount The length of our binary number. Defaults to 8.
     * @return The binary number as a string.
     */
    std::uniform_int_distribution<int> distrib(0, 1);
    std::string binary_string = "";
    for (int i = 0; i < length; i++) {
        binary_string += std::to_string(distrib(GENERATOR));
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

    std::uniform_real_distribution<double> distribution(0.0, max);

    std::vector<float> energies(size);
    for (int i = 0; i < size; ++i) {
        energies[i] = distribution(GENERATOR);
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
    for (int i = 0; i < binString.size(); ++i) {
        if (std::generate_canonical<double, 5>(GENERATOR) < p)
            newString[i] = binString[i] == '0' ? '1' : '0';
        else
            newString[i] = binString[i];
    }
    return newString;
}

std::vector<std::string> listOfFood(unsigned int length,
                                    unsigned int food_size /*= FOOD_SIZE*/) {
    /**
     * Generates a list of random binary strings that represent the
     *  food available for a population.
     * @param length The length of the list.
     * @param food_size The size of the binary strings. Defaults to FOOD_SIZE=8.
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

void printMap(mapa_prot& mapa) {
    /**
     * Prints a map.
     * @param mapa The map to be printed.
     */
    for (auto const& pair : mapa) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

void printMap(mapa_met& mapa) {
    /**
     * Prints a map.
     * @param mapa The map to be printed.
     */
    for (auto const& pair : mapa) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

void printMap(mapa_dim& mapa) {
    /**
     * Prints a map.
     * @param mapa The map to be printed.
     */
    for (auto const& pair : mapa) {
        std::cout << "{" << pair.first.id << ": " << pair.second << "}\n";
    }
}

void printMap(mapa_owm& mapa) {
    /**
     * Prints a map.
     * @param mapa The map to be printed.
     */
    std::cout << "Printing mapa_owm\n";

    for (auto const& pair : mapa) {
        std::cout << "{Prot: " << pair.first.prot
                  << ", Dim: " << pair.first.dim.id
                  << ", Met: " << pair.first.met << ": " << pair.second
                  << "}\n";
    }
}

std::pair<mapa_met, mapa_met> mapDistribution(mapa_met mapa, bool method_1,
                                              float p) {
    /**
     * Shuffles a map. We can choose between two methods of dealing the map. One
     * where we give a component to each child map in an orderly fashion
     * (method_1) and the other where we throw a coin for each component to
     * decide which child map it goes to (method_2).
     * @param mapa The map to be shuffled.
     * @param method_1 If true, we use method_1. If false, we use method_2.
     * @return The shuffled map.
     */
    mapa_met map1;
    mapa_met map2;
    if (mapa.empty()) {
        return std::make_pair(map1, map2);
    }

    int i = 0;
    for (auto const& pair : mapa) {
        if (((i % 2 == 0) && method_1) ||
            ((std::generate_canonical<double, 5>(GENERATOR) < p) &&
             !method_1)) {
            map1[pair.first] = pair.second;
        } else {
            map2[pair.first] = pair.second;
        }
        i++;
    }
    return std::make_pair(map1, map2);
}

std::pair<mapa_prot, mapa_prot> mapDistribution(mapa_prot mapa, bool method_1,
                                                float p) {
    /**
     * Shuffles a map. We can choose between two methods of dealing the map. One
     * where we give a component to each child map in an orderly fashion
     * (method_1) and the other where we throw a coin for each component to
     * decide which child map it goes to (method_2).
     * @param mapa The map to be shuffled.
     * @param method_1 If true, we use method_1. If false, we use method_2.
     * @return The shuffled map.
     */
    mapa_prot map1;
    mapa_prot map2;
    if (mapa.empty()) {
        return std::make_pair(map1, map2);
    }
    int i = 0;
    for (auto const& pair : mapa) {
        if (((i % 2 == 0) && method_1) ||
            ((std::generate_canonical<double, 5>(GENERATOR) < p) &&
             !method_1)) {
            map1[pair.first] = pair.second;
        } else {
            map2[pair.first] = pair.second;
        }
        i++;
    }
    return std::make_pair(map1, map2);
}

std::pair<mapa_dim, mapa_dim> mapDistribution(mapa_dim mapa, bool method_1,
                                              float p) {
    /**
     * Shuffles a map. We can choose between two methods of dealing the map. One
     * where we give a component to each child map in an orderly fashion
     * (method_1) and the other where we throw a coin for each component to
     * decide which child map it goes to (method_2).
     * @param mapa The map to be shuffled.
     * @param method_1 If true, we use method_1. If false, we use method_2.
     * @return The shuffled map.
     */
    mapa_dim map1;
    mapa_dim map2;
    if (mapa.empty()) {
        return std::make_pair(map1, map2);
    }
    int i = 0;
    for (auto const& pair : mapa) {
        if (((i % 2 == 0) && method_1) ||
            ((std::generate_canonical<double, 5>(GENERATOR) < p) &&
             !method_1)) {
            map1[pair.first] = pair.second;
        } else {
            map2[pair.first] = pair.second;
        }
        i++;
    }
    return std::make_pair(map1, map2);
}