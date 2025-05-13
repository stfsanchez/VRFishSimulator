/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "utils.h"

#include <sstream>
#include <algorithm>

/**
 * split function
 *
 * Split a string into tokens according to a delimiter string
 *
 * @param s     :   The string to be split
 * @param delim :   The string to use as a delimiter
 * @return  the tokens extracted from the string to split
 */
vector<string> split(const string& s, const string& delim)
{
    vector<string> result;

    // Handle the cas of an empty delimiter
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }

    // Iterate over the string
    string::const_iterator substart = s.begin(), subend;
    while (true) {
        // Search for match of delim in string
        subend = search(substart, s.end(), delim.begin(), delim.end());
        string temp(substart, subend);
        if (!temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

/**
 * stringToNumber function
 *
 * Convert a string to the double it represents
 *
 * @param Text  :   The string to be converted
 * @return  the double converted from the string
 */
double stringToNumber(const string &Text)
{
    double d;
    stringstream ss(Text); //turn the string into a stream
    ss >> d; //convert
    return d;
}
