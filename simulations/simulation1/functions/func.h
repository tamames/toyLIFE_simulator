#pragma once

#include <random>
#include <string>
#include <stdexcept>
#include <algorithm> // std::count std::max_element
#include <numeric>   // std::accumulate
#include <fstream>

std::string DIRECTORY = "C:\\Users\\arcos\\Documents\\Universidad\\Master\\TFM\\codigos\\simulations\\simulation1";

std::string binaryGenerator(int length = 8, float control = 0.5f)
{
    /**
     * Generates a random binary number as a string.
     * We can control the distribution of 1s and 0s with the control parameter.
     * @param amount The length of our binary number. Defaults to 8.
     * @param control The probability of a 1 appearing in the binary number. Defaults to 0.5.
     * @return The binary number as a string.
     */
    std::string binary_string = "";
    for (int i = 0; i < length; i++)
    {
        float numero = (float)rand() / (float)RAND_MAX;
        binary_string += numero < control ? '1' : '0';
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

std::vector<std::string> listFood(unsigned int length)
{
    /**
     * Generates a list of random binary strings that represent the
     *  food available for a population.
     * @param length The length of the list.
     * @return The list of random binary strings.
     */

    std::vector<std::string> listOFFood(length);
    for (int i = 0; i < length; ++i)
        listOFFood[i] = binaryGenerator();

    return listOFFood;
}

void statistic(std::vector<std::string> listOFBinaries)
{
    /**
     * Prints the statistics of the a vector of binaries such a list of food or a list or genotypes.
     * Such as the percentage of 1s in total and the
     * maximum and minimum number of 1s in a binary string.
     * @param listOFBinaries The list of binary strings.
     */

    int totalSize = listOFBinaries.size();
    int eachSize = listOFBinaries[0].size();
    std::vector<int> ones(totalSize, 0);
    std::vector<int> zeros(totalSize, 0);
    for (int i = 0; i < totalSize; ++i)
    {
        ones[i] = std::count(listOFBinaries[i].begin(), listOFBinaries[i].end(), '1');
        zeros[i] = eachSize - ones[i];
    }

    int totalOnes = std::accumulate(ones.begin(), ones.end(), 0);
    int totalZeros = std::accumulate(zeros.begin(), zeros.end(), 0);

    auto maxOne = std::max_element(ones.begin(), ones.end());
    int maxOneValue = *maxOne;
    int maxOneIndex = std::distance(ones.begin(), maxOne);

    std::cout << "The maximum number of 1s in a binary string is " << maxOneValue << "\n";
    std::cout << "And it's obtain in the index " << maxOneIndex << ": " << listOFBinaries[maxOneIndex] << "\n";
    std::cout << "The number of 1s in the food list is " << totalOnes << "\n";
    std::cout << "The number of 0s in the food list is " << totalZeros << "\n";
    std::cout << "The number of elements is " << totalSize * eachSize << "\n";
    std::cout << totalOnes + totalZeros << "\n";

    // std::cout << "The food list has " << totalSize << " elements.\n";
    // std::cout << "The food list has " << ones << " ones.\n";
    // std::cout << "The food list has " << zeros << " zeros.\n";
}

void printVector(std::vector<std::string> vector2Print)
{
    /**
     * Prints a vector of binary strings.
     * @param vector2Print The list of binary strings.
     */

    for (int i = 0; i < vector2Print.size(); ++i)
        std::cout << vector2Print[i] << "\n";
}

void writeResults(std::string fileName, std::string description)
{
    /**
     * Writes the results of the simulation in a file.
     * First the function writes into a description file to keep a record of what is inside each file.
     * We are going to write the results in the following format:
     *     The first line is going to be some header for the file
     *     In the next lines we are going to write the statistics of the population for each iteration.
     */

    // First we open the description file to write what is going to have the file.
    std::ofstream desFile;
    std::string desFileDir = DIRECTORY + "\\data\\" + "Description.md";
    desFile.open(desFileDir, std::ios::app);

    if (!desFile)
    {
        std::cout << "Impossible to open the description file.";
        exit(EXIT_FAILURE);
    }

    desFile << "\n"
            << fileName + ".txt"
            << " -> "
            << description
            << "\n";

    desFile.close();

    std::cout << "Writing the results in " << fileName << "...\n";

    std::string path = DIRECTORY + "\\data\\" + fileName + ".txt";
    std::ofstream myFile;
    myFile.open(path, std::ios::out);

    if (!myFile)
    {
        std::cout << "There was an error while opening your file\n";
        exit(EXIT_FAILURE);
    }

    // myFile << description << "\n";
    myFile.close();
    std::cout << "Finished writing the results.\n";
}