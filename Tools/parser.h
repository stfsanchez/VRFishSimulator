/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "mainwindow.h"
#include "Core/simulator.h"
#include "utils.h"

using namespace std;


class MainWindow;

class Parser
{
public:

    static void parseReplayData(string filepath, Simulator* sim);

    static void parseNeighborsData(string filepath, Simulator* sim, int k);

};

#endif // PARSER_H
