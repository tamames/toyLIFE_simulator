#include "classes.h"
#include "functions/func.h"
#include "functions/globals.h"
#include "toylife/toy_plugin.h"

int main() {
    std::cout << "Hello world!" << std::endl;

    ToyPlugin toy;

    std::cout << "Se crea el toy" << std::endl;

    Agent test(10.0);
    std::cout << "Se crea el agent" << std::endl;

    std::map<std::string, int> food;
    food["01101010"] = 1;

    test.print();
    test.eat(food, toy);
    test.print();

    // std::system("pause");
}
