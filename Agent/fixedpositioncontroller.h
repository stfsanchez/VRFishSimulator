/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/


#ifndef FIXEDPOSITIONCONTROLLER_H
#define FIXEDPOSITIONCONTROLLER_H

#include "controller.h"



class FixedPositionController : public Controller
{
public:
    int num_steps;

    bool fixedz = true;
    int referencefishid = 0;

    //Controller data
    double speed;
    double theta;
    double newtheta;
    double angleToFish;
    double distanceToFish;

    //Fish data
    double newx, newy, newz;
    double newphi, newchi;
    double newspeed;
    double dl_next;

    FixedPositionController();

    void swim();

    void initFish();
    void act(double dt);
    void update();
    void reset();

    double rndm();
    double angle_to_pipi(double difference) const;


};

#endif // FIXEDPOSITIONCONTROLLER_H
