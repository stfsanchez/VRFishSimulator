/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "RNGSingleton.h"

#include "rngconstants.h"

// Singleton instance accessor
RNGSingleton& RNGSingleton::getInstance() {
    static RNGSingleton instance;  // Single instance
    return instance;
}

// Private constructor (seeded once)
RNGSingleton::RNGSingleton() : generator(seed) {} // Seed can be changed

// Method to generate a random number between 0.0 and 1.0
double RNGSingleton::getRandomNumber() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

// Method to generate a random number between 0.0 and 1.0
double RNGSingleton::getGaussianRandomNumber() {
    static std::normal_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}


/* ran3: random number generator (translated from Fortran) */
double RNGSingleton::ran3()
{
    long mj;
    int i, k;
    if (idum < 0 || iff == 0) {
        iff = 1;
        mj = labs(MSEED - labs(idum));
        mj %= MBIG;
        ma[55] = mj;
        long mk = 1;
        for (i = 1; i <= 54; i++) {
            int ii = (21 * i) % 55;
            ma[ii] = mk;
            mk = mj - mk;
            if (mk < 0) mk += MBIG;
            mj = ma[ii];
        }
        for (k = 1; k <= 4; k++) {
            for (i = 1; i <= 55; i++) {
                ma[i] -= ma[1 + (i + 30) % 55];
                if (ma[i] < 0) ma[i] += MBIG;
            }
        }
        inext = 0;
        inextp = 31;
        idum = 1;
    }
    if (++inext == 56) inext = 1;
    if (++inextp == 56) inextp = 1;
    mj = ma[inext] - ma[inextp];
    if (mj < 0) mj += MBIG;
    ma[inext] = mj;
    return mj * FAC + ma[inextp] * FAC2;
}
