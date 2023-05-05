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

void fillRandomArray(int *arr, int size, int max = 8)
{
    /**
     * Fills an array with random integers.
     * @param arr The array to be filled.
     * @param size The size of the array.
     * @param max The maximum value of the random integers. Defaults to 8.
     */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, max);

    for (int i = 0; i < size; ++i)
    {
        arr[i] = dis(gen);
    }
}

std::string mutate(std::string bin_string, double p) {
    if (p < 0 || p > 1) {
        throw std::invalid_argument("The probability must be between 0 and 1");
    }
    std::string new_string = "";
    std::random_device rd;
    std::mt19937 gen(rd());
    for (char bit : bin_string) {
        if (std::generate_canonical<double, 5>(gen) < p) {
            new_string += std::to_string(!std::stoi(std::string(1, bit)));
        } else {
            new_string += bit;
        }
    }
    return new_string;
}