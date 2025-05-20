/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "fishtank.h"

using namespace std;

FishTank::FishTank(double x, double y, double radius, int id) {
    tankID = id;
    cx = x; cy = y;
    this->radius = radius;
}



FishTank::~FishTank() {
}
