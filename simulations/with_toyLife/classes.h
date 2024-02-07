#ifndef CLASSES_H  // include guard
#define CLASSES_H

#include <algorithm>  // std::count std::min
#include <bitset>     // std::bitset
#include <iostream>
#include <stdexcept>  // std::invalid_argument
#include <string>
#include <vector>

// my functions and variables
#include "functions/func.h"
#include "functions/globals.h"

// toyLife code
#include "toylife/helper_functions.h"
#include "toylife/toy_plugin.h"

class Agent {
   public:
    std::string genotype;
    float energy;
    int age;
    int id;
    int parent;  // If it's 0 it's from the first generation

    // toyLife things
    mapa_prot prots;
    mapa_dim dims;
    mapa_met mets;
    mapa_owm owns;

    Agent(float energy, std::string genotype = "",
          int sizeGenotype = SIZE_GENOTYPE, int parent = 0,
          mapa_prot prots = {}, mapa_dim dims = {}, mapa_met mets = {});

    void print(bool printAge = false);

    bool checkEnergyReproduce();

    bool checkDie();

    bool checkPDM();

    void reacting(const ToyPlugin& toy);

    void promoter_expression(const ToyPlugin& toy);

    void eat(std::map<std::string, int>& food, const ToyPlugin& toy);

    bool metabolism(std::map<std::string, int>& food, const ToyPlugin& toy);

    std::vector<std::string> getAgentData();
};

std::pair<Agent, Agent> divide(Agent parent, float p);

class Population {
   public:
    int generation;
    int sizePopulation;
    std::vector<Agent> agents;

    // For statistics
    std::vector<float> averageEnergyGain;
    std::vector<float> averageEnergyLoss;
    std::vector<int> maxEnergyGain;
    std::vector<int> minEnergyGain;

    Population(int sizePopulation);

    void print();

    void iteration(std::vector<std::string> food, ToyPlugin toy,
                   float cost = 1.0, double costVariation = 0.1,
                   bool print = false);

    void afterIteration(float p);

    void deleteElements(std::vector<Agent>& agents, std::vector<int> indexes);

    void addAges();

    std::vector<std::string> getGenotypes();

    std::vector<std::vector<std::string>> getPopulationData();
};

#endif  // CLASSES_H
