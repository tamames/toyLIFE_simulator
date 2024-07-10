#include "func.h"

#include <algorithm>
#include <bitset>
#include <ctime>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <stdexcept>
#include <string>

#include "../toylife/toy_plugin.h"
#include "globals.h"

std::string binaryGenerator(int length, float control) {
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

std::string mutate(std::string binString) {
    /**
     * Mutates a binary string.
     * @param binString The binary string to be mutated.
     * @return The mutated binary string.
     */

    if (MUTATION_PROBABILITY < 0.0f || MUTATION_PROBABILITY > 1.0f) {
        std::cout << "The probability must be between 0 and 1\n";
        throw std::invalid_argument("The probability must be between 0 and 1");
    }

    std::string newString(binString.size(), ' ');
    for (int i = 0; i < binString.size(); ++i) {
        if (std::generate_canonical<double, 5>(GENERATOR) <
            MUTATION_PROBABILITY)
            newString[i] = binString[i] == '0' ? '1' : '0';
        else
            newString[i] = binString[i];
    }
    return newString;
}

std::vector<std::string> listOfFood(
    unsigned int length, unsigned int food_size /*= SIZE_EACH_FOOD*/) {
    /**
     * Generates a list of random binary strings that represent the
     *  food available for a population.
     * @param length The length of the list.
     * @param food_size The size of the binary strings. Defaults to
     * SIZE_EACH_FOOD=8.
     * @return The list of random binary strings.
     */

    std::vector<std::string> listOfFood(length);
    for (int i = 0; i < length; ++i)
        listOfFood[i] = binaryGenerator(food_size);

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
        map[list[i]] += 1;

    return map;
}

// Function to randomly select N keys from the map
std::vector<std::string> randomSelectFood(
    const std::map<std::string, int>& foodMap, int N) {
    std::vector<std::string> keys;
    keys.reserve(foodMap.size());

    for (const auto& pair : foodMap) {
        keys.push_back(pair.first);
    }

    std::shuffle(keys.begin(), keys.end(), GENERATOR);  // Shuffle keys randomly
    keys.resize(
        std::min(N, static_cast<int>(keys.size())));  // Select first N keys

    return keys;
}

void subtractFromFoodMap(std::map<std::string, int>& foodMap,
                         const std::vector<std::string>& keys) {
    /**
     * Substracts 1 from the value of the keys in the foodMap.
     * If the value is 0, the key is removed from the map.
     * @param foodMap The map of food.
     * @param keys The keys to substract from the map.
     */
    for (const auto& key : keys) {
        foodMap[key]--;  // Decrease value by 1
        if (foodMap[key] == 0) {
            foodMap.erase(key);  // Mark key for deletion
        }
    }
}

void addKeysToFoodMap(
    std::map<std::string, int>& foodMap,
    const std::vector<std::map<std::string, int>>& returnedFood,
    const std::vector<std::string>& newFood) {
    /**
     * Adds the keys to the foodMap.
     * @param foodMap The map of food.
     * @param keys The keys to add to the map.
     */

    // First we add the new food
    for (const auto& key : newFood) {
        foodMap[key]++;
    }

    // Then we add the returned food
    for (const auto& map : returnedFood) {
        for (const auto& pair : map) {
            foodMap[pair.first] += pair.second;
        }
    }
}

std::map<std::string, int> sampleFood(std::map<std::string, int>& foodMap) {
    /**
     * Selects N food from the foodMap, converts them to a map and removes then
     * from the foodMap.
     * @param foodMap The map of food.
     * @return The selected food.
     */
    std::map<std::string, int> selectedFood;
    std::vector<std::string> keys = randomSelectFood(foodMap);
    for (const auto& key : keys) {
        selectedFood[key]++;
    }
    subtractFromFoodMap(foodMap, keys);
    return selectedFood;
}

void foodWriting(const std::vector<std::string>& foodVector,
                 std::filesystem::path folderPath, int iteration, char mode) {
    /** Writes all the food into a file just to plot a histogram of the
     * distribution I had an idea about plotting the food but I am realizing
     * that this can also be used to plot the genotypes
     *
     * @param foodList The list of food or genotypes that we want to write.
     */

    std::filesystem::path total_path = folderPath / "food.csv";

    std::ofstream myFile;

    if (iteration == 0)
        // it is the first iteration so we create the file
        myFile.open(total_path, std::ios::out);
    else
        // we call the function
        myFile.open(total_path, std::ios::app);

    if (!myFile) {
        std::cout << "There was an error while opening your file.\n";
        exit(EXIT_FAILURE);
    }

    if (iteration == 0) {
        myFile << "Iteration,Binary,Mode\n";
    }

    for (std::size_t i = 0; i < foodVector.size(); ++i) {
        myFile << std::to_string(iteration) << "," << foodVector[i] << ","
               << mode << "\n";
    }

    myFile.close();
}

void foodWriting(const std::vector<mapa_met>& foodMap,
                 std::filesystem::path folderPath, int iteration, char mode) {
    /** Writes all the food into a file just to plot a histogram of the
     * distribution I had an idea about plotting the food but I am realizing
     * that this can also be used to plot the genotypes
     *
     * @param foodList The list of food or genotypes that we want to write.
     */

    std::filesystem::path total_path = folderPath / "food.csv";

    std::ofstream myFile;

    if (iteration == 0)
        // it is the first iteration so we create the file
        myFile.open(total_path, std::ios::out);
    else
        // we call the function
        myFile.open(total_path, std::ios::app);

    if (!myFile) {
        std::cout << "There was an error while opening your file.\n";
        exit(EXIT_FAILURE);
    }

    if (iteration == 0) {
        myFile << "Iteration,Binary,Mode\n";
    }

    for (std::size_t i = 0; i < foodMap.size(); ++i) {
        for (auto const& pair : foodMap[i]) {
            // we iterate over the map consisting on food: amount
            for (int j = 0; j < pair.second; ++j) {
                // we write the food once per amount there is
                myFile << std::to_string(iteration) << "," << pair.first << ","
                       << mode << "\n";
            }
        }
    }

    myFile.close();
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

void writeResults(std::string fileName, std::filesystem::path folderPath,
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
     * @param folder_path The folder where we write the file.
     * @param headers The headers of the file.
     * @param results The data that we want to write into the file.
     */
    // First we check that we have the same amount of headers as columns in
    // results
    int headers_size = headers.size();
    int results_size = results[0].size();
    if (headers_size != results_size) {
        std::cout << "The headers and the results don't have the same size.\n";
        exit(EXIT_FAILURE);
    }

    // Now we deal with the results file.
    // We check if the file already exists, so we don't lose any data.
    std::filesystem::path total_path = folderPath / (fileName + ".csv");
    if (std::filesystem::exists(total_path)) {
        std::cout << "The file " << fileName
                  << " already exists in the designated folder.\n";
        std::cout << "Do you want to overwrite it? (y/n)\n";
        char answer;
        std::cin >> answer;
        if (answer == 'y') {
            std::cout << "Overwriting the file...\n";
            std::filesystem::remove(DIRECTORY / ("data/" + fileName + ".csv"));
        } else {
            std::cout << "The file was not overwritten.\n";
            return;
        }
    }

    std::cout << "Writing the results in " << fileName << "...\n";
    std::ofstream myFile;
    myFile.open(total_path, std::ios::out);

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

void populationWriting(std::vector<std::vector<std::string>> dataOfPopulation,
                       int iteration, std::filesystem::path folder_path) {
    /**This is a modification of the writeResults function.
     * Basically I want to write all the information about the population in
     * each generation. We call this function each time for storage purposes and
     * to prevent that if the simulation crashes we lose all the data. The csv
     * contains one row for each agent in each generation. And the data
     * displayed is the one we obtain from the getAgentData function.
     */

    std::filesystem::path total_path = folder_path / "total.csv";
    std::ofstream myFile;

    myFile.open(total_path, std::ios::app);

    if (!myFile) {
        std::cout << "There was an error while opening your file.\n";
        exit(EXIT_FAILURE);
    }

    if (iteration == 0) {
        // If we are in the first iteration we write the headers.

        std::vector<std::string> headers = {"ID",   "Genotype", "Energy",
                                            "Age",  "Parent",   "Prots",
                                            "Mets", "Dims",     "Iteration"};
        // First we write the headers
        for (std::size_t i = 0; i < headers.size(); ++i) {
            myFile << headers[i];
            if (i < headers.size() - 1)
                myFile << ";";
        }
        myFile << "\n";
    }

    // Then we write the data of the population

    for (std::size_t i = 0; i < dataOfPopulation.size(); ++i) {
        for (std::size_t j = 0; j < dataOfPopulation[i].size(); ++j) {
            myFile << dataOfPopulation[i][j];
            if (j < dataOfPopulation[i].size())
                myFile << ";";
        }
        myFile << std::to_string(iteration);
        myFile << "\n";
    }
    myFile.close();
}

int getNumberOfSimulation() {
    /**
     * Read the simulations\with_toyLife\functions\number_of_simulation.txt file
     * and return the number in it.
     * @return The number of the simulation.
     */

    std::filesystem::path numberSimulationsPath =
        "functions/number_of_simulation.txt";

    std::ifstream file;
    file.open(DIRECTORY / numberSimulationsPath);
    if (!file) {
        std::cout << "There was an error while opening the file:\n"
                  << (DIRECTORY / numberSimulationsPath) << "\n";
        exit(EXIT_FAILURE);
    }
    int number;
    file >> number;
    file.close();
    return number;
}

void increaseNumberOfSimulation() {
    /**
     * Increases the number of the simulation by one.
     */

    std::filesystem::path numberSimulationsPath =
        "functions/number_of_simulation.txt";

    std::ofstream file;
    file.open(DIRECTORY / numberSimulationsPath);
    if (!file) {
        std::cout << "There was an error while opening the file:\n"
                  << (DIRECTORY / numberSimulationsPath) << "\n";
        exit(EXIT_FAILURE);
    }
    file << NUMBER_OF_SIMULATION + 1;
    file.close();
}

std::filesystem::path createDataDirectory() {
    /**
     * Creates a directory to store the data of the corresponding simulation.
     * In this directory we have to have a Readme.md file that store the
     * constants of the simulation. If the directory already exists, the
     * function asks if we want to overwrite it. This would happened in case the
     * previous simulation ended abruptly.
     */

    // first we check if the parent directory exists
    if (!std::filesystem::exists(DIRECTORY / "data")) {
        std::filesystem::create_directory(DIRECTORY / "data");
    }

    std::filesystem::path simulationFolderRelativePath(
        "data/simulation_" + std::to_string(NUMBER_OF_SIMULATION));

    std::filesystem::path simulationFolderPath =
        DIRECTORY / simulationFolderRelativePath;

    if (std::filesystem::exists(simulationFolderPath)) {
        std::cout << "The directory  " << std::to_string(NUMBER_OF_SIMULATION)
                  << " already exists.\n";
        std::cout << "Do you want to overwrite it? (y/n)\n";
        char answer;
        std::cin >> answer;
        if (answer == 'y') {
            std::cout << "Overwriting the directory...\n";
            std::filesystem::remove_all(simulationFolderPath);
        } else {
            std::cout << "The directory was not overwritten.\n";
            exit(EXIT_FAILURE);
        }
    }
    std::filesystem::create_directory(simulationFolderPath);

    return simulationFolderPath;
}

void createReadMe(int numberOfGenerations, int initPopulationSize,
                  int foodSize) {
    /**
     * Creates and fill the ReadMe file inside the data directory
     */

    std::filesystem::path simulationFolderRelativePath(
        "data/simulation_" + std::to_string(NUMBER_OF_SIMULATION));

    std::filesystem::path simulationFolderPath =
        DIRECTORY / simulationFolderRelativePath;

    if (!std::filesystem::exists(simulationFolderPath)) {
        std::cout << "The " << simulationFolderRelativePath
                  << " directory doesn't exist.\n";
        exit(EXIT_FAILURE);
    }

    std::ofstream file;
    file.open(simulationFolderPath / "Readme.md");

    if (!file) {
        std::cout << "There was an error while opening the file:\n"
                  << simulationFolderPath << "Readme.md" << "\n";
        exit(EXIT_FAILURE);
    }
    file << "## Parameters of the simulation: \n";
    file << "* **Number of generations** &rarr; " << numberOfGenerations << "\n"
         << "* **Initial population size** &rarr; " << initPopulationSize
         << "\n"
         << "* **Maximum population size** &rarr; " << MAXIMUM_POPULATION_SIZE
         << "\n"
         << "* **Energy to reproduce** &rarr; " << ENERGY_TO_REPRODUCE << "\n"
         << "* **Energy to die** &rarr; " << ENERGY_TO_DIE << "\n"
         << "* **Age to die** &rarr; " << AGE_TO_DIE << "\n"
         << "* **Translation energy** &rarr; " << TRANSLATION_ENERGY << "\n"
         << "* **Breaking energy** &rarr; " << BREAKING_ENERGY << "\n"
         << "* **Size of the genotype** &rarr; " << SIZE_GENOTYPE << "\n"
         << "* **Size of each food** &rarr; " << SIZE_EACH_FOOD << "\n"
         << "* **Control** &rarr; " << CONTROL << "\n"
         << "* **Mutation probability** &rarr; " << MUTATION_PROBABILITY << "\n"
         << "* **Original Food Size** &rarr; " << foodSize << "\n"
         << "* **Sample size** &rarr; " << SAMPLE_SIZE << "\n"
         << "* **Food to add** &rarr; " << FOOD_TO_ADD << "\n";
}

std::string fromMapToString(mapa_prot& mapa) {
    /**
     * Converts a map to a string. The prot is an integer.
     * @param mapa The map to be converted.
     * @return The string.
     */

    if (mapa.empty())
        return "";
    std::string out = "{";
    for (auto const& pair : mapa) {
        out += std::to_string(pair.first) + ": " + std::to_string(pair.second) +
               ", ";
    }
    // to remove the last comma and space
    out.pop_back();
    out.pop_back();

    out += "}";
    return out;
}

std::string fromMapToString(mapa_met& mapa) {
    /**
     * Converts a map to a string. In this case the met is already a string.
     * @param mapa The map to be converted.
     * @return The string.
     */

    if (mapa.empty())
        return "";
    std::string out = "{";
    for (auto const& pair : mapa) {
        out += pair.first + ": " + std::to_string(pair.second) + ", ";
    }
    out.pop_back();
    out.pop_back();
    out += "}";
    return out;
}

std::string fromMapToString(mapa_dim& mapa) {
    /**
     * Converts a map to a string. In this case the dim is an object so we take
     * the id.
     * @param mapa The map to be converted.
     * @return The string.
     */

    if (mapa.empty())
        return "";
    std::string out = "{";
    for (auto const& pair : mapa) {
        out += std::to_string(pair.first.id) + ": " +
               std::to_string(pair.second) + ", ";
    }
    out.pop_back();
    out.pop_back();
    out += "}";
    return out;
}

std::string currentTime() {
    /**
     * Returns the current hour, minute and second in a string.
     * @return The string with the current hour, minute and second.
     */
    std::time_t now = time(0);
    std::tm* ltm = localtime(&now);

    // Format the time as HH:MM:SS
    std::stringstream ss;
    ss << std::put_time(ltm, "%H:%M:%S");

    return ss.str();
}