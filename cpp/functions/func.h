#include <random>
#include <string>

std::string binary_generator(int length = 8)
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
