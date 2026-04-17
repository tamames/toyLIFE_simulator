#include "globals.h"

std::string DIRECTORY =
    "C:"
    "\\Users\\arcos\\Documents\\Universidad\\Master\\TFM\\codigos\\simulations"
    "\\simulation1";
int SIZE_GENOTYPE = 20;
float CONTROL =
    0.25f;  // The probability of a 1 appearing in the binary string.

int NUMBER_OF_SIMULATION = 10;  // Jus to to keep track of which simulation we
                                // are doing

int ENERGY_TO_REPRODUCE = 15;
int ENERGY_TO_DIE = 0;
int AGE_TO_DIE =
    100;  // If it's above the number of generations it doesn't matter
int ID_COUNT = 0;