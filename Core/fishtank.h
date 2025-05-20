/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef FISHTANK_H
#define FISHTANK_H

#include <vector>


class FishTank
{
public:
    int tankID;
    double cx, cy;
    double radius;

    FishTank(double x, double y, double radius, int id);

    ~FishTank();
};

#endif // FISHTANK_H
