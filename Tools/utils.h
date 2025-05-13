/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
using namespace std;


struct Point{
    double x;
    double y;
    double z;

    Point(double px, double py, double pz): x(px), y(py), z(pz) {}
};

/**
 * split function
 *
 * Split a string into tokens according to a delimiter string
 *
 * @param s     :   The string to be split
 * @param delim :   The string to use as a delimiter
 * @return  the tokens extracted from the string to split
 */
vector<string> split(const string& s, const string& delim);


/**
 * stringToNumber function
 *
 * Convert a string to the double it represents
 *
 * @param Text  :   The string to be converted
 * @return  the double converted from the string
 */
double stringToNumber(const string &Text);


#endif // UTILS_H
