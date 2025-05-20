/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef ROSACECONTROLLER_H
#define ROSACECONTROLLER_H

#include "controller.h"

class RosaceController : public Controller
{
public:
    bool initDone;

    int num_steps;
    bool fixedz = true;

    //Controller data
    double K;
    bool reverse;
    double speed;
    double omega;
    double radius;
    double theta;
    double th;

    //Fish data
    double newx, newy, newz;
    double newphi, newchi;
    double newspeed;
    double newtheta;
    double dl_next;


    RosaceController();

    void initFish();
    void act(double dt);
    void update();
    void reset();

    void swim();
    void setReverse(bool pValue);
    void setMeanSpeed(double pSpeed);

};

#endif // ROSACECONTROLLER_H
