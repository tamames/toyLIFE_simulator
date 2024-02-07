#ifndef GLOBALS_H
#define GLOBALS_H

// We are going to define here all the global variables of our simulation
#include <filesystem>
#include <random>
#include <string>

extern std::string DIRECTORY;
extern int SIZE_GENOTYPE;
extern int FOOD_SIZE;
extern float CONTROL;  // The probability of a 1 appearing in the binary string.

extern int NUMBER_OF_SIMULATION;  // Jus to to keep track of which simulation we
                                  // are doing

extern int ENERGY_TO_REPRODUCE;
extern int ENERGY_TO_DIE;
extern int AGE_TO_DIE;
extern int ID_COUNT;
extern float TRANSLATION_ENERGY;
extern float BREAKING_ENERGY;

extern std::random_device rd;
extern std::mt19937 GEN(rd());

#endif  // GLOBALS_H
