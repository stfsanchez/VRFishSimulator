/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

using namespace std;

class Fish;
class Simulator;
class Controller
{
public:
    Fish * fish;
    Simulator * sim;

    string type;
    bool active = true;

    bool onePassAction = true;

    Controller();
    virtual ~Controller(){};

    virtual void linkFish(Fish * mFish) { fish = mFish; }
    virtual void linkSimulator(Simulator * mSim) { sim = mSim; }

    virtual void initFish() = 0;
    virtual void act(double dt)=0;
    virtual void update()=0;
    virtual void reset()=0;
};

#endif // CONTROLLER_H
