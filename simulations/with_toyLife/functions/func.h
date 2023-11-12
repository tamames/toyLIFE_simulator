#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#pragma once

#include "globals.h"

std::string binaryGenerator(int length = 8);

std::vector<float> createRandomArray(int size, int max = 8);

std::string mutate(std::string binString, float p);

std::vector<std::string> listOfFood(unsigned int length = FOOD_SIZE);

#endif  // FUNC_H