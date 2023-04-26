#include <string>
#include <vector>
#include <stdexcept>
using std::string;

class Agent
{
    float Energy;
    string Genotype;

public:
    Agent(float energy, string genotype)
    {
        Energy = energy;
        Genotype = genotype;
    }

    bool checkEnergy2Reproduce()
    {
        return Energy > 10;
    }

    bool checkEnergy2Die()
    {
        return Energy < 0;
    }

    // void eat()
    // {
    // }
};

// class Population()
// {
//     int Generation = 0;
//     vector Individuals;

// public:
//     Population()
//     {
//     }
// };
// class Agent {
// public:
//     Agent(float energy, int size_genotype = 8, std::string genotype = "") {
//         if (genotype.empty()) {
//             this->genotype = binary_generator();
//         }
//         else {
//             this->genotype = genotype;
//         }
//         this->energy = energy;
//         this->age = 0;
//     }
//     std::string get_genotype() {
//         return genotype;
//     }
//     float get_energy() {
//         return energy;
//     }
//     void set_energy(float energy) {
//         this->energy = energy;
//     }
//     int get_age() {
//         return age;
//     }
//     void set_age(int age) {
//         this->age = age;
//     }
//     std::string to_string() {
//         return "Genotype: " + genotype + ". Energy: " + std::to_string(energy);
//     }
//     bool check_energy_reproduce() {
//         return energy >= 10;
//     }
//     bool check_energy_die() {
//         return energy < 0;
//     }
//     void eat(std::string food) {
//         int genotype_int = std::stoi(genotype, nullptr, 2);
//         int food_int = std::stoi(food, nullptr, 2);
//         int eaten_int = genotype_int & food_int;
//         std::string eaten = std::bitset<8>(eaten_int).to_string();
//         energy += std::count(eaten.begin(), eaten.end(), '1');
//     }
// private:
//     std::string genotype;
//     float energy;
//     int age;
// };
