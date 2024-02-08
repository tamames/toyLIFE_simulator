#include "globals.h"

std::string DIRECTORY = std::filesystem::current_path().generic_string();
int SIZE_GENOTYPE = 60;
int FOOD_SIZE = 8;
float CONTROL = 0.5f;  // The probability of a 1 appearing in the binary string.

int NUMBER_OF_SIMULATION = 0;  // Jus to to keep track of which simulation we
                               // are doing

int ENERGY_TO_REPRODUCE = 15;
int ENERGY_TO_DIE = 0;
int AGE_TO_DIE =
    100;  // If it's above the number of generations it doesn't matter
int ID_COUNT = 0;
float TRANSLATION_ENERGY = 0.0f;
float BREAKING_ENERGY = 5.0f;

std::random_device rd;
std::mt19937 GENERATOR(rd());