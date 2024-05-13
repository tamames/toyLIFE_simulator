#include "globals.h"

#include "func.h"

std::string DIRECTORY = std::filesystem::current_path().generic_string();
int SIZE_GENOTYPE = 80;

int INITIAL_FOOD_SIZE = 1000;
int SIZE_EACH_FOOD = 8;
float CONTROL = 0.5f;  // The probability of a 1 appearing in the binary string.
int SAMPLE_SIZE = 7;   // the amount of food each individual is going to eat
int FOOD_TO_ADD = 300;  // The amount of food to add each iteration

int NUMBER_OF_SIMULATION =
    getNumberOfSimulation();  // Just to to keep track of which simulation we
                              // are doing
int ID_COUNT = 1;

int ENERGY_TO_REPRODUCE = 20;
int ENERGY_TO_DIE = 0;
int AGE_TO_DIE =
    10000;  // If it's above the number of generations it doesn't matter
float TRANSLATION_ENERGY = 0.1f;
float BREAKING_ENERGY = 30.0f;

std::random_device rd;
std::mt19937 GENERATOR(rd());

float MUTATION_PROBABILITY = 0.01f;

int NUMBER_OF_GENERATIONS = 70;
int INITIAL_POPULATION_SIZE = 150;
