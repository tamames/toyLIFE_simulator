#include "classes.h"
#include "functions/func.h"
#include "functions/globals.h"
#include "toylife/toy_plugin.h"

int main() {
    std::cout << "Hello world!" << std::endl;

    ToyPlugin toy;

    Agent test(10.0);

    std::map<std::string, int> food;
    food["01101010"] = 1;

    test.eat(food, toy);

    std::system("pause");
}
