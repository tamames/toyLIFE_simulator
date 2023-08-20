#pragma once

#include <algorithm>  // std::count std::max_element
#include <numeric>    // std::accumulate
#include <random>
#include <stdexcept>
#include <string>

const std::string DIRECTORY =
    "C:"
    "\\Users\\arcos\\Documents\\Universidad\\Master\\TFM\\codigos\\simulations"
    "\\simulation1";
const int SIZE_GENOTYPE = 20;

std::string binaryGenerator(int length = 20, float control = 0.5f) {
    /**
     * Generates a random binary number as a string.
     * We can control the distribution of 1s and 0s with the control parameter.
     * @param amount The length of our binary number. Defaults to 20.
     * @param control The probability of a 1 appearing in the binary number.
     * Defaults to 0.5.
     * @return The binary number as a string.
     */
    std::string binary_string = "";
    for (int i = 0; i < length; i++) {
        float numero = (float)rand() / (float)RAND_MAX;
        binary_string += numero < control ? '1' : '0';
    }
    return binary_string;
}

std::vector<float> createRandomArray(int size, int max = 8) {
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

std::vector<std::string> listFood(unsigned int length, float control = 0.5f) {
    /**
     * Generates a list of random binary strings that represent the
     *  food available for a population.
     * @param length The length of the list.
     * @param control The probability of a 1 appearing in the binary number.
     * @return The list of random binary strings.
     */

    std::vector<std::string> listOFFood(length);
    for (int i = 0; i < length; ++i)
        listOFFood[i] = binaryGenerator(SIZE_GENOTYPE, control);

    return listOFFood;
}

std::vector<std::string> statistic(std::vector<std::string> listOFBinaries) {
    /**
     * Prints the statistics of the a vector of binaries such a list of food or
     * a list or genotypes. Such as the percentage of 1s in total and the
     * maximum and minimum number of 1s in a binary string.
     * @param listOFBinaries The list of binary strings.
     *                       For instance it could be the food given to a
     *                       population or the genotypes of a population.
     */

    std::size_t totalSize = listOFBinaries.size();
    std::size_t eachSize =
        listOFBinaries[0]
            .size();  // we assume that each binary string has the same size.
    int totalElements = totalSize * eachSize;

    // First the amount of 1s and 0s in each binary string.
    std::vector<int> ones(totalSize, 0);
    std::vector<int> zeros(totalSize, 0);
    for (int i = 0; i < totalSize; ++i) {
        ones[i] =
            std::count(listOFBinaries[i].begin(), listOFBinaries[i].end(), '1');
        zeros[i] = eachSize - ones[i];
    }

    int totalOnes = std::accumulate(ones.begin(), ones.end(), 0);
    int totalZeros = std::accumulate(zeros.begin(), zeros.end(), 0);

    auto maxOne = std::max_element(ones.begin(), ones.end());
    int maxOneValue = *maxOne;
    int maxOneIndex = std::distance(ones.begin(), maxOne);

    std::vector<std::string> results = {
        std::to_string(maxOneValue), std::to_string(maxOneIndex),
        listOFBinaries[maxOneIndex], std::to_string(totalOnes),
        std::to_string(totalZeros),  std::to_string(totalElements)};

    return results;
}

template <typename T>
void printVector(const std::vector<T> &vector2Print) {
    /**
     * Prints the elements of a vector.
     * @param vector2Print The vector to be printed.
     */

    for (const T &element : vector2Print) {
        std::cout << element << "\n";
    }
}

#include <filesystem>  // std::filesystem::exists
#include <fstream>
void writeResults(std::string fileName, std::string description,
                  std::vector<std::string> headers,
                  std::vector<std::vector<std::string>> results) {
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
    std::ofstream desFile;
    std::string desFileDir = DIRECTORY + "\\data\\" + "Readme.md";
    desFile.open(desFileDir, std::ios::app);

    if (!desFile) {
        std::cout << "Impossible to open the description file.";
        exit(EXIT_FAILURE);
    }

    desFile << fileName + ".csv"
            << " &rarr; " << description << "\n";

    desFile.close();

    // Now we deal with the results file.
    // We check if the file already exists, so we don't lose any data.
    const std::string path = DIRECTORY + "\\data\\" + fileName + ".csv";
    if (std::filesystem::exists(path)) {
        std::cout << "The file " << fileName << " already exists.\n";
        std::cout << "Do you want to overwrite it? (y/n)\n";
        char answer;
        std::cin >> answer;
        if (answer == 'y') {
            std::cout << "Overwriting the file...\n";
            std::filesystem::remove(DIRECTORY + "\\data\\" + fileName + ".csv");
        } else {
            std::cout << "The file was not overwritten.\n";
            return;
        }
    }

    std::cout << "Writing the results in " << fileName << "...\n";
    std::ofstream myFile;
    myFile.open(path, std::ios::out);

    if (!myFile) {
        std::cout << "There was an error while opening your file.\n";
        exit(EXIT_FAILURE);
    }

    // First we write the headers
    for (std::size_t i = 0; i < headers.size(); ++i) {
        myFile << headers[i];
        if (i < headers.size() - 1)
            myFile << ",";
    }

    myFile << "\n";

    // Then we write the results
    for (std::size_t i = 0; i < results.size(); ++i) {
        for (std::size_t j = 0; j < results[i].size(); ++j) {
            myFile << results[i][j];
            if (j < results[i].size() - 1)
                myFile << ",";
        }
        myFile << "\n";
    }

    myFile.close();
    std::cout << "Finished writing the results.\n";
}