#include "classes.h"
#include "functions/func.h"

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