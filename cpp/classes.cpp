#include <iostream>
#include <string>
#include <vector>
#include <stdexcept> // std::invalid_argument
#include <bitset>    // std::bitset
#include <algorithm> // std::count
#include "functions/func.h"

class Agent
{
public:
    std::string genotype;
    float energy;
    int size_genotype;
    int age;

    Agent(float energy, std::string genotype = "", int size_genotype = 8)
    {
        this->size_genotype = size_genotype;
        if (genotype.empty())
            this->genotype = binaryGenerator(this->size_genotype);
        else
            this->genotype = genotype;

        this->energy = energy;
        this->age = 0;
    }

    void print()
    {
        std::cout << "Genotype: " << genotype << ". Energy: " << energy << std::endl;
    }

    bool checkEnergyReproduce()
    {
        return energy >= 10;
    }
    bool checkEnergyDie()
    {
        return energy < 5;
    }
    void eat(std::string food)
    {
        /**
         * Describe the interaction between an Agent and the food.
         *We make an AND operation between the two binary strings and
         *the energy gained by the Agent is the number of '1' that the
         *final string has.
         * @param food The food that the Agent is going to eat.
         */
        int genotype_int = std::stoi(genotype, nullptr, 2);
        int food_int = std::stoi(food, nullptr, 2);
        int eaten_int = genotype_int & food_int;
        std::string eaten = std::bitset<8>(eaten_int).to_string();
        this->energy += std::count(eaten.begin(), eaten.end(), '1');
    }
};

class Population
{
public:
    int generation = 0;
    int size_population;
    std::vector<Agent> agents;
    Population(int size_population)
    {
        this->size_population = size_population;
        this->agents = std::vector<Agent>();
        // int energies[size_population];
        std::vector<int> energies = createRandomArray(size_population, 8);
        for (int i = 0; i < size_population; ++i)
        {
            Agent agent(energies[i]);
            agents.push_back(agent);
        }
    }

    void print()
    {
        std::cout << "Population: \n";
        for (int i = 0; i < size_population; ++i)
        {
            agents[i].print();
        }
    }

    void iteration(std::vector<std::string> food, int sizeFood, float cost)
    {
        for (Agent i : agents) // every agent search for food
            i.energy -= cost;

        // generate random order
        std::vector<int> order(sizeFood);
        std::iota(order.begin(), order.end(), 0);
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(order.begin(), order.end(), g);

        if (size_population < sizeFood) // maybe there is not enough food to everybody
            std::cout << "There is not enough food for everybody.\n";

        for (int i = 0; i < sizeFood; ++i)
            agents[order[i]].eat(food[i]);

        generation++;
        addAges();
    }

    void afterIteration(float p)
    {
        /**
         * Here we check the energy of each agent and do the
         *corresponding action. First we check the ones that died.
         *Then we check the ones that can reproduce and we reproduce them.
         * @param p The probability of mutation.
         */

        std::vector<int> deadsPositions; // store the indexes of the deads
        for (int i = 0; i < size_population; ++i)
        {
            if (agents[i].checkEnergyDie())
                deadsPositions.push_back(i);
        }

        // std::cout << "Before the remove: " << agents.size() << std::endl;
        // std::cout << "To remove: " << deadsPositions.size() << std::endl;

        // deleteElements(agents, deadsPositions);
        // // std::cout << "After the remove: " << agents.size() << std::endl;

        // std::vector<int> reproducePositions; // store the indexes of the ones to reproduce
        // for (int i = 0; i < size_population; ++i)
        // {
        //     if (agents[i].checkEnergyReproduce())
        //         reproducePositions.push_back(i);
        // }

        // std::vector<Agent> newElements(reproducePositions.size() * 2);
        // for (int i = 0; i < reproducePositions.size(); ++i)
        // {
        //     std::pair<Agent, Agent> children = divide(agents[reproducePositions[i]], p);
        //     newElements[i * 2] = children.first;
        //     newElements[i * 2 + 1] = children.second;
        // }

        // deleteElements(agents, reproducePositions);
        // agents.insert(agents.end(), newElements.begin(), newElements.end());
    }

    void deleteElements(std::vector<Agent> &agents, std::vector<int> indexes)
    {
        for (auto it = indexes.rbegin(); it != indexes.rend(); it++)
            agents.erase(agents.begin() + *it);
    }

    void addAges()
    {
        for (Agent i : agents)
            i.age++;
    }
};

std::pair<Agent, Agent> divide(Agent parent, int p)
{
    /**
     * Divide an agent into two agents.
     * @param parent The agent that is going to be divided.
     * @param p The probability of the children of being mutated.
     */

    int energyC = parent.energy / 2;
    std::string genotype1 = mutate(parent.genotype, p);
    std::string genotype2 = mutate(parent.genotype, p);

    Agent child1(energyC, genotype1);
    Agent child2(energyC, genotype2);

    return std::make_pair(child1, child2);
}

int main()
{
    Population popu(10);
    popu.print();
}
