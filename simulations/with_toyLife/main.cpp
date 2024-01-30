#include "classes.h"
#include "functions/func.h"
#include "functions/globals.h"
#include "toylife/toy_plugin.h"

int main() {
    std::cout << "Hello world!" << std::endl;

    ToyPlugin toy;
    std::cout << "Se crea el toyPlugin" << std::endl;

    std::string gen =
        "111101010001110110101100100100100000101110111010101101011011";

    std::map<std::string, int> foodMap;
    foodMap["10000001"] = 1;

    Agent test(100.0, gen);
    test.eat(foodMap, toy);
    std::cout << "End" << std::endl;
}

// int main() {
//     std::cout << "Hello world!" << std::endl;

//     ToyPlugin toy;

//     std::cout << "Se crea el toyPlugin" << std::endl;
//     std::vector<std::string> food = decimal2Binary(256);

//     // abrimos el archivo para guardar
//     std::ofstream desFile;
//     std::string desFileDir =
//         "C:"
//         "\\Users\\arcos\\Documents\\Universidad\\Master\\TFM\\codigos\\simulati"
//         "ons\\with_toyLife\\log.txt";
//     desFile.open(desFileDir, std::ios::app);

//     if (!desFile) {
//         std::cout << "Impossible to open the description file.";
//         exit(EXIT_FAILURE);
//     }

//     for (int j = 0; j < 1000000; ++j) {
//         Agent test(100.0);
//         for (auto i : food) {
//             std::map<std::string, int> foodMap;
//             foodMap[i] = 1;
//             bool a = test.metabolism(foodMap, toy);

//             if (a) {
//                 desFile << "Ha ganado energía."
//                         << "\n";
//                 desFile << "La comida era: " << i << "\n";
//                 desFile << "El gen era: " << test.genotype << "\n";
//             }
//         }
//         if (j % 1000 == 0) {
//             desFile << "Iteracion: " << j << "\n";
//             // std::cout << "Iteracion: " << j << "\n";
//         }
//     }

//     desFile << "Termina el programa"
//             << "\n";
//     desFile.close();
//     std::cout << "Termina el programa" << std::endl;

//     // int count_mas = 0;
//     // int count_menos = 0;

//     // for (int i = 0; i < 2000; i++) {
//     //     Agent test(10.0);
//     //     test.eat(food, toy);
//     //     test.eat(food, toy);
//     //     if (test.energy == 10.0) {
//     //         continue;
//     //     } else if (test.energy < 10.0) {
//     //         count_menos++;
//     //     } else {
//     //         count_mas++;
//     //     }
//     // }
//     // std::cout << "Menos: " << count_menos << "\n";
//     // std::cout << "Mas: " << count_mas << "\n";
//     // std::system("pause");
// }
