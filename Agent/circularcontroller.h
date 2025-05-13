/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef CIRCULARCONTROLLER_H
#define CIRCULARCONTROLLER_H

#include "controller.h"

using namespace std;

class CircularController : public Controller
{
public:
    //Burst and coast data
    bool burstandcoast;
    bool initDone;
    int phase; //0: burst, 1: coast
    bool kicking;
    int num_kick;
    double kicktime;
    double kickspeed;
    double kicktheta;
    double tau;
    double newtau;

    int num_steps;

    bool fixedz = true;
    int referencefishid = 0;

    //Controller data
    bool reverse;
    double speed;
    double omega;
    double radius;
    double theta;

    //Fish data
    double newx, newy, newz;
    double newphi, newchi;
    double newspeed;
    double newtheta;
    double dl_next;

    CircularController();

    void swim();
    void glide();
    void kick();
    bool checkKick();


    void initFish();
    void act(double dt);
    void update();
    void reset();

    double rndm();
    double angle_to_pipi(double difference) const;

    void setReverse(bool pValue);
    void setMeanSpeed(double pSpeed);
};

#endif // CIRCULARCONTROLLER_H
