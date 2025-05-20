/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef RNGSINGLETON_H
#define RNGSINGLETON_H

#include <random>

class RNGSingleton {
public:
    static RNGSingleton& getInstance();  // Singleton access method
    double getRandomNumber();            // Generate a random number in [0.0, 1.0]
    double getGaussianRandomNumber();            // Generate a random number in [0.0, 1.0]
    double ran3();

private:
    RNGSingleton();                       // Private constructor
    RNGSingleton(const RNGSingleton&) = delete;
    RNGSingleton& operator=(const RNGSingleton&) = delete;

    std::mt19937 generator;               // Mersenne Twister random engine
};

#endif // RNGSINGLETON_H
