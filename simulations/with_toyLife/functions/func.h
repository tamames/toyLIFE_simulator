#ifndef FUNC_H
#define FUNC_H

#include <algorithm>
#include <bitset>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <stdexcept>
#include <string>
#pragma once

#include "../toylife/toy_plugin.h"
#include "globals.h"

std::string binaryGenerator(int length = 8);

std::vector<float> createRandomArray(int size, int max = 8);

std::string mutate(std::string binString, float p);

std::vector<std::string> listOfFood(unsigned int length,
                                    unsigned int food_size = SIZE_EACH_FOOD);

std::map<std::string, int> fromList2Map(std::vector<std::string> list);

std::vector<std::string> decimal2Binary(int numbers);

void printMap(mapa_prot& mapa);
void printMap(mapa_met& mapa);
void printMap(mapa_dim& mapa);
void printMap(mapa_owm& mapa);

std::pair<mapa_met, mapa_met> mapDistribution(mapa_met mapa,
                                              bool method_1 = true,
                                              float p = 0.5f);
std::pair<mapa_prot, mapa_prot> mapDistribution(mapa_prot mapa,
                                                bool method_1 = true,
                                                float p = 0.5f);
std::pair<mapa_dim, mapa_dim> mapDistribution(mapa_dim mapa,
                                              bool method_1 = true,
                                              float p = 0.5f);

std::vector<std::string> sampleFood(std::vector<std::string>& food,
                                    unsigned int sample_size = SAMPLE_SIZE);

void writeResults(std::string fileName, std::string folder_path,
                  std::vector<std::string> headers,
                  std::vector<std::vector<std::string>> results);

void populationWriting(std::vector<std::vector<std::string>> dataOfPopulation,
                       int iteration, std::string folder_path);

void foodWriting(std::vector<std::string> foodList, std::string folderPath);

int getNumberOfSimulation();
void increaseNumberOfSimulation();

std::string createDataDirectory();
void createReadMe(int numberOfGenerations, int initialPopulationSize,
                  int foodSize);

std::string fromMapToString(mapa_prot& mapa);
std::string fromMapToString(mapa_met& mapa);
std::string fromMapToString(mapa_dim& mapa);

#endif  // FUNC_H