/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>

#include "fishtank.h"
#include "Agent/fish.h"

#define ERROR 0.000001

using namespace std;

class Simulator
{
public:
    FishTank* tank = NULL;
    vector<Fish*> fish;

    int currentFishID;

    int nbIcpt;
    int activeIndex;

    int xpSteps;
    int simSteps;
    double initTime;
    double actualTime;
    double previousTime;
    double dt;

    bool pause;

    //Fishschool data
    double school_x;
    double school_y;
    double last_school_x;
    double last_school_y;
    double school_xmin;
    double school_xmax;
    double school_ymin;
    double school_ymax;
    double school_dir;
    int school_center_x;
    int school_center_y;

    Simulator();
    Simulator(double initTime);
    ~Simulator();

    void addFish(Fish *f);
    void addTank(FishTank * ft);

    void oneStep();

    bool simulationFinished();

    void reset();

    void computeFishData();

    void computeSchoolCenter();

    void setActualTime(float pTime);

    void setDt(double pDt);

};

#endif // SIMULATOR_H
