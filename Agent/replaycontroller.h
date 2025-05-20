/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef REPLAYCONTROLLER_H
#define REPLAYCONTROLLER_H

#include "controller.h"
#include "Tools/utils.h"

class ReplayController:public Controller
{
public:
    vector<Point> positions;
    vector<double> directions;
    vector<vector<int>> neighbors;

    unsigned int replayIndex = 0;

    int neighbors_frame = 0;

    ReplayController();
    ~ReplayController();

    void addPosition(Point &newPoint);
    void addDirection(double newDir);
    void addNeighbors(vector<int> newNeighbors);

    void initFish();
    void act(double dt);
    void update();
    void reset();
};

#endif // REPLAYCONTROLLER_H
