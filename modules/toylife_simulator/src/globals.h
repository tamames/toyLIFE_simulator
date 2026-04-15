#ifndef GLOBALS_H
#define GLOBALS_H

// We are going to define here all the global variables of our simulation
#include <filesystem>
#include <random>
#include <string>

extern std::filesystem::path DIRECTORY;
extern int SIZE_GENOTYPE;

extern int INITIAL_FOOD_SIZE;
extern int SIZE_EACH_FOOD;
extern float CONTROL;  // The probability of a 1 appearing in the binary string.
extern int SAMPLE_SIZE;  // the amount of food each individual is going to eat
extern int FOOD_TO_ADD;  // The amount of food to add each iteration

extern int NUMBER_OF_SIMULATION;  // Jus to to keep track of which simulation we
                                  // are doing

extern int ID_COUNT;

extern int ENERGY_TO_REPRODUCE;
extern int ENERGY_TO_DIE;
extern int AGE_TO_DIE;
extern float TRANSLATION_ENERGY;
extern float BREAKING_ENERGY;

extern std::random_device rd;
extern std::mt19937 GENERATOR;

extern float MUTATION_PROBABILITY;

extern int NUMBER_OF_GENERATIONS;
extern int INITIAL_POPULATION_SIZE;
extern int MAXIMUM_POPULATION_SIZE;

#endif  // GLOBALS_H
