#include <algorithm>  // std::count std::min std::max_element std::min_element
#include <iostream>
#include <stdexcept>  // std::invalid_argument
#include <string>
#include <vector>

#include "functions/func.h"

// Some global variables
const int ENERGY_TO_REPRODUCE = 10;
const int ENERGY_TO_DIE = 5;

class Agent {
   public:
    std::string genotype;
    float energy;
    int age;

    Agent(float energy, std::string genotype = "") {
        if (genotype.empty()) {
            this->genotype = binaryGenerator(SIZE_GENOTYPE);
        } else {
            if (genotype.size() != SIZE_GENOTYPE)  // to prevent errors
                throw std::invalid_argument("The genotype must be of size " +
                                            std::to_string(SIZE_GENOTYPE) +
                                            ".");

            this->genotype = genotype;
        }
        this->energy = energy;
        this->age = 0;
    }

    void print(bool printAge = false) {
        if (printAge)
            std::cout << "Genotype: " << genotype << ". Energy: " << energy
                      << ". Age:" << age << std::endl;
        else
            std::cout << "Genotype: " << genotype << ". Energy: " << energy
                      << std::endl;
    }

    bool checkEnergyReproduce() { return energy >= ENERGY_TO_REPRODUCE; }

    bool checkEnergyDie() { return energy < ENERGY_TO_DIE; }

    int eat(std::string food) {
        /**
         * Describe the interaction between an Agent and the food.
         * The more similar the genotype and the food are, the more energy
         * the Agent gains.
         * @param food The food that the Agent is going to eat.
         * @return The energy that the Agent gains.
         *         We return this parameter to gain data about the simulation.
         */

        int energy_gain = 0;

        for (int i = 0; i < food.size(); ++i) {
            if (food[i] == genotype[i])
                energy_gain++;
        }
        this->energy += energy_gain;

        return energy_gain;
    }
};

std::pair<Agent, Agent> divide(Agent parent, float p) {
    /**
     * Divide an agent into two agents.
     * @param parent The agent that is going to be divided.
     * @param p The probability of the children of being mutated.
     */

    float energyC = parent.energy / 2;
    std::string genotype1 = mutate(parent.genotype, p);
    std::string genotype2 = mutate(parent.genotype, p);

    Agent child1(energyC, genotype1);
    Agent child2(energyC, genotype2);

    return std::make_pair(child1, child2);
}

class Population {
   public:
    int generation = 0;
    int sizePopulation;
    std::vector<Agent> agents;

    // For statistics
    std::vector<float> averageEnergyGain;
    std::vector<float> averageEnergyLoss;
    std::vector<int> maxEnergyGain;
    std::vector<int> minEnergyGain;

    Population(int sizePopulation) {
        this->sizePopulation = sizePopulation;
        this->agents = std::vector<Agent>();
        std::vector<float> energies = createRandomArray(sizePopulation, 15);
        for (int i = 0; i < sizePopulation; ++i) {
            Agent agent(energies[i]);
            agents.push_back(agent);
        }
    }

    void print() {
        std::cout << "You are printing the whole population: \n";
        for (int i = 0; i < sizePopulation; ++i) {
            agents[i].print();
        }
    }

    void iteration(std::vector<std::string> food, float cost,
                   double costVariation = 0.1, bool print = false) {
        /**
         * Here we do the iteration of the population. First we
         *make the agents search for food. Then we generate a random
         *order to eat the food. Then we check the energy of each agent
         *and do the corresponding action. First we check the ones that died.
         *Then we check the ones that can reproduce and we reproduce them.
         * @param food The food that the agents are going to eat.
         * @param cost The cost of searching for food.
         * @param costVariation The variation of the cost so every agent
         *                      doesn't spend the same energy searching for
         *food.
         * @param print If true it logs extra information.
         */

        int sizeFood = food.size();

        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_real_distribution<double> variationDist(
            -cost * costVariation, cost * costVariation);

        for (Agent &i : agents) {  // every agent search for food
            double randomCost = cost + variationDist(g);
            i.energy -= randomCost;
        }

        if ((sizePopulation > sizeFood) && print)
            std::cout << "There is not enough food for everybody.\n";

        int smaller = std::min(sizePopulation, sizeFood);

        // generate random order
        std::vector<int> order(smaller);
        std::iota(order.begin(), order.end(), 0);
        std::shuffle(order.begin(), order.end(), g);

        std::vector<float>
            gainedEnergies;  // store the energies gained by each agent

        for (int i = 0; i < smaller; ++i) {
            Agent &agent = agents[order[i]];

            if (agent.checkEnergyDie())  // When searching for food the agent
                                         // died.
                continue;
            gainedEnergies.push_back(agent.eat(food[i]));
        }

        // Fill the vectors for statistics.
        int sum =
            std::accumulate(gainedEnergies.begin(), gainedEnergies.end(), 0);
        averageEnergyGain.push_back(static_cast<double>(sum) / (float)smaller);

        auto maxGain =
            std::max_element(gainedEnergies.begin(), gainedEnergies.end());
        maxEnergyGain.push_back(*maxGain);

        auto minGain =
            std::min_element(gainedEnergies.begin(), gainedEnergies.end());
        minEnergyGain.push_back(*minGain);

        generation++;
        addAges();
    }

    void afterIteration(float p) {
        /**
         * Here we check the energy of each agent and do the
         *corresponding action. First we check the ones that died.
         *Then we check the ones that can reproduce and we reproduce them.
         * @param p The probability of mutation.
         */

        std::vector<int> deadsPositions;  // store the indexes of the deads

        for (int i = 0; i < sizePopulation; ++i) {
            if (agents[i].checkEnergyDie())
                deadsPositions.push_back(i);
        }

        deleteElements(agents, deadsPositions);

        std::vector<int>
            reproducePositions;  // store the indexes of the ones to reproduce
        for (int i = 0; i < sizePopulation; ++i) {
            if (agents[i].checkEnergyReproduce())
                reproducePositions.push_back(i);
        }

        std::vector<Agent> newElements(reproducePositions.size() * 2, Agent(0));
        for (int i = 0; i < reproducePositions.size(); ++i) {
            std::pair<Agent, Agent> children =
                divide(agents[reproducePositions[i]], p);
            newElements[i * 2] = children.first;
            newElements[i * 2 + 1] = children.second;
        }

        deleteElements(agents, reproducePositions);
        agents.insert(agents.end(), newElements.begin(), newElements.end());
        this->sizePopulation =
            agents.size();  // update the size of the population
    }

    void deleteElements(std::vector<Agent> &agents, std::vector<int> indexes) {
        for (auto it = indexes.rbegin(); it != indexes.rend(); it++)
            agents.erase(agents.begin() + *it);

        this->sizePopulation =
            agents.size();  // update the size of the population
    }

    void addAges() {
        for (Agent &i : agents)
            i.age++;
    }

    std::vector<std::string> getGenotypes() {
        std::vector<std::string> genotypes(sizePopulation);
        for (int i = 0; i < sizePopulation; ++i)
            genotypes[i] = agents[i].genotype;

        return genotypes;
    }
};
