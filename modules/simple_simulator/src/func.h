#pragma once

#include <algorithm>  // std::count std::max_element
#include <iostream>
#include <numeric>  // std::accumulate
#include <random>
#include <stdexcept>
#include <string>

#include "globals.h"

std::string binaryGenerator(int length = 20, float control = 0.5f);
    /**
     * Generates a random binary number as a string.
     * We can control the distribution of 1s and 0s with the control parameter.
     * @param amount The length of our binary number. Defaults to 20.
     * @param control The probability of a 1 appearing in the binary number.
     * Defaults to 0.5.
     * @return The binary number as a string.
     */


std::vector<float> createRandomArray(int size, int max = 8);
    /**
     * Creates a vector and fill it with random numbers.
     * @param size The size of the array.
     * @param max The maximum value of the random integers. Defaults to 8.
     */
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dis(1, max);



std::string mutate(std::string binString, float p);
    /**
     * Mutates a binary string.
     * @param binString The binary string to be mutated.
     * @param p The probability of mutation.
     * @return The mutated binary string.
     */


std::vector<std::string> listFood(unsigned int length, float control = 0.5f);
    /**
     * Generates a list of random binary strings that represent the
     *  food available for a population.
     * @param length The length of the list.
     * @param control The probability of a 1 appearing in the binary number.
     * @return The list of random binary strings.
     */


std::vector<std::string> statistic(std::vector<std::string> listOFBinaries) ;
    /**
     * Prints the statistics of the a vector of binaries such a list of food or
     * a list or genotypes. Such as the percentage of 1s in total and the
     * maximum and minimum number of 1s in a binary string.
     * @param listOFBinaries The list of binary strings.
     *                       For instance it could be the food given to a
     *                       population or the genotypes of a population.
     * @return A vector of strings with the following statistics:
     *                      1 - The maximum number of ones
     *                      2 - The index where this maximum is obtained
     *                      3 - The binary string with the maximum # of ones
     *                      4 - The total number of ones
     *                      5 - The total number of zeros
     *                      6 - The total number of elements
     */


template <typename T>
void printVector(const std::vector<T>& vector2Print) {
    /**
     * Prints the elements of a vector.
     * @param vector2Print The vector to be printed.
     */

    for (const T& element : vector2Print) {
        std::cout << element << "\n";
    }
}

#include <filesystem>  // std::filesystem::exists
#include <fstream>
void writeResults(std::string fileName, std::string description,
                  std::vector<std::string> headers,
                  std::vector<std::vector<std::string>> results) ;
    /**
     * Writes the results of the simulation in a file.
     * First the function writes into a description file to keep a record of
     * what is inside each file. We are going to write the results in the
     * following format: The first line is going to be some header for the file
     * In the next lines we are going to write the data.
     *
     * @param fileName The name of the file.
     * @param description A description of the file.
     * @param headers The headers of the file.
     * @param results The data that we want to write into the file.
     */

    // First we open the description file to write what is going to have the
    // file.


void foodWriting(std::vector<std::string> foodList);
    /** Writes all the food into a file just to plot a histogram of the
     * distribution I had an idea about plotting the food but I am realizing
     * that this can also be used to plot the genotypes
     *
     * @param foodList The list of food or genotypes that we want to write.
     */



void populationWriting(std::vector<std::vector<std::string>> dataOfPopulation,
                       int iteration);
    /**This is a modification of the writeResults function.
     * Basically I want to write all the information about the population in
     * each generation, The csv contains one row for each agent in each
     * generation. And the data displayed is the following: An id to identify
     * the agent The genotype of the agent The energy of the agent The age of
     * the agent The net energy of that simulation.
     *
     */




// Now we introduce some functions to compute the similarities between the
// genotypes of a population.

// float jaccardSimilarity(const std::string& str1, const std::string& str2) {
//     int intersection = 0;
//     int unionSize = 0;

//     for (int i = 0; i < str1.size(); ++i) {
//         if (str1[i] == '1' && str2[i] == '1') {
//             intersection++;
//         }
//         if (str1[i] == '1' || str2[i] == '1') {
//             unionSize++;
//         }
//     }

//     if (unionSize == 0) {
//         return 0.0f;  // Handle case where both strings are all zeros
//     }

//     return static_cast<float>(intersection) / static_cast<float>(unionSize);
// }
