#include <random>
#include <string>
#include <stdexcept>

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

std::vector<int> createRandomArray(int size, int max = 8)
{
    /**
     * Creates a vector and fill it with random numbers.
     * @param size The size of the array.
     * @param max The maximum value of the random integers. Defaults to 8.
     */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, max);

    std::vector<int> energies(size);

    for (int i = 0; i < size; ++i)
    {
        energies[i] = dis(gen);
    }
    return energies;
}

std::string mutate(std::string bin_string, double p)
{
    /**
     * Mutates a binary string.
     * @param bin_string The binary string to be mutated.
     * @param p The probability of mutation.
     * @return The mutated binary string.
     */

    if (p < 0 || p > 1)
        throw std::invalid_argument("The probability must be between 0 and 1");

    std::string newString(bin_string.size(), ' ');
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < bin_string.size(); ++i)
    {
        if (std::generate_canonical<double, 5>(gen) < p)
            newString[i] = bin_string[i] == '0' ? '1' : '0';
        else
            newString[i] = bin_string[i];
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

    std::vector<std::string> listFood(length);
    for (int i = 0; i < length; ++i)
        listFood[i] = binaryGenerator();

    return listFood;
}