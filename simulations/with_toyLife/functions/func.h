#ifndef FUNC_H
#define FUNC_H

#include <iostream>
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

std::vector<std::string> listOfFood(unsigned int length = FOOD_SIZE);

std::map<std::string, int> fromList2Map(std::vector<std::string> list);

std::vector<std::string> decimal2Binary(int numbers);

void printMap(mapa_prot& mapa);
void printMap(mapa_met& mapa);
void printMap(mapa_dim& mapa);
void printMap(mapa_owm& mapa);
#endif  // FUNC_H