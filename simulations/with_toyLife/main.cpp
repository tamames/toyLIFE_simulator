#include "classes.h"
#include "functions/func.h"


std::string DIRECTORY;
int SIZE_GENOTYPE;
int FOOD_SIZE;
float CONTROL;  // The probability of a 1 appearing in the binary string.

int NUMBER_OF_SIMULATION;  // Jus to to keep track of which simulation we
// are doing

int ENERGY_TO_REPRODUCE;
int ENERGY_TO_DIE;
int AGE_TO_DIE;
int ID_COUNT;
float TRANSLATION_ENERGY;
float BREAKING_ENERGY;


int main() {
    std::cout << "Hello world!" << std::endl;

    // para probar si funciona el crear una instancia de la clase Agent
    Agent foo(10.0);

    // para probar si funciona el binaryGenerator
    std::string a = binaryGenerator();
    std::cout << a << std::endl;

    // Los errores que me aparecen a mi son:
    // undefined reference to `Agent::Agent(float,
    // std::__cxx11::basic_string<char, std::char_traits<char>,
    // std::allocator<char> >, int)'
    // y
    // undefined reference to
    // `binaryGenerator[abi:cxx11](int)'
}
