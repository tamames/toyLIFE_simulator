#include "classes.h"
#include "functions/func.h"
#include "functions/globals.h"
#include "toylife/toy_plugin.h"

int main() {
    std::cout << "Hello world!" << std::endl;

    ToyPlugin toy;

    std::cout << "Se crea el toyPlugin" << std::endl;

    std::map<std::string, int> food;
    food["01101010"] = 1;

    int count_mas = 0;
    int count_menos = 0;

    for (int i = 0; i < 2000; i++) {
        Agent test(10.0);
        test.eat(food, toy);
        test.eat(food, toy);
        if (test.energy == 10.0) {
            continue;
        } else if (test.energy < 10.0) {
            // std::cout << "Menos energia " << test.energy << "\n";
            count_menos++;
        } else {
            // std::cout << "Más energia " << test.energy << "\n";
            count_mas++;
        }
    }
    std::cout << "Menos: " << count_menos << "\n";
    std::cout << "Mas: " << count_mas << "\n";
    // std::system("pause");
}
