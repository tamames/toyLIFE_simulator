#include "classes.h"
#include "functions/func.h"
#include "functions/globals.h"
#include "toylife/toy_plugin.h"

int main() {
    std::cout << "Hello world!" << std::endl;

    ToyPlugin toy;

    std::cout << "Se crea el toyPlugin" << std::endl;

    Agent test(100.0);
    std::vector<std::string> food = decimal2Binary(256);
    for (auto i : food) {
        std::map<std::string, int> foodMap;
        foodMap[i] = 1;
        bool a = test.metabolism(foodMap, toy);
        if (a) {
            std::cout << "Ha ganado energía."
                      << "\n";
            std::cout << "La comida era: " << i << "\n";
        }
    }
    std::cout << "Termina el programa" << std::endl;

    // int count_mas = 0;
    // int count_menos = 0;

    // for (int i = 0; i < 2000; i++) {
    //     Agent test(10.0);
    //     test.eat(food, toy);
    //     test.eat(food, toy);
    //     if (test.energy == 10.0) {
    //         continue;
    //     } else if (test.energy < 10.0) {
    //         count_menos++;
    //     } else {
    //         count_mas++;
    //     }
    // }
    // std::cout << "Menos: " << count_menos << "\n";
    // std::cout << "Mas: " << count_mas << "\n";
    // std::system("pause");
}
