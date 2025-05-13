/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "fixedpositioncontroller.h"

#include "controller.h"
#include "../Core/simulator.h"
#include "controller2dconstants.h"
#include "Tools/rngsingleton.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

FixedPositionController::FixedPositionController()
{
    type = "fixedposition";
    this->onePassAction = true;
}

void FixedPositionController::initFish(){
    this->num_steps = 0;

    this->distanceToFish = fixed_distance;
    this->angleToFish = angle_to_pipi(fixed_angle * M_PI/180.);
    this->fixedz = fixed_fixedz;
    this->referencefishid = fixed_referencefishid;

    this->newx = 0.0;
    this->newy = 0.0;
    if (this->fixedz){
        this->newz = fixed_zmean;
    }
    else{
        this->newz = 0.;
    }

    this->newphi = 0.; //angle_to_pipi(this->newtheta + M_PI/2);
    this->newchi = 0;

    this->fish->active = true;
}

void FixedPositionController::swim(){
    this->newspeed = this->speed;

    Fish* ref_fish = this->sim->fish[this->referencefishid];
    this->newtheta = angle_to_pipi(ref_fish->phi + this->angleToFish);

    this->newx = ref_fish->x + (this->distanceToFish*std::cos(this->newtheta)); //cm !
    this->newy = ref_fish->y + (this->distanceToFish*sin(this->newtheta)); //cm !

    if (this->fixedz){
        this->newz = fixed_zmean;
    }
    else{
        this->newz = ref_fish->z;
    }

    this->newphi = ref_fish->phi;
    this->newchi = 0;
}


void FixedPositionController::act(double dt){
    this->num_steps ++;
    this->swim();
}

void FixedPositionController::update(){
    this->speed = this->newspeed;
    this->fish->x = this->newx;
    this->fish->y = this->newy;
    this->fish->z = this->newz;
    this->fish->chi = this->newchi;
    this->fish->phi = this->newphi;
    this->fish->v = this->newspeed;
    this->theta = this->newtheta;

    this->speed = 0.;
    this->fish->x = 0.;
    this->fish->y = 0.;
    this->fish->z = 5.;
    this->fish->chi = 0.;
    this->fish->phi = 0.;
    this->fish->v = 0.;
    this->theta = 0.;
}

void FixedPositionController::reset(){
    this->initFish();
}


double FixedPositionController::rndm(){
    RNGSingleton& rng = RNGSingleton::getInstance();
    return rng.getRandomNumber();
}

double FixedPositionController::angle_to_pipi(double difference) const
{
    do {
        if (difference < -M_PI)
            difference += 2. * M_PI;
        if (difference > M_PI)
            difference -= 2. * M_PI;
    } while (std::abs(difference) > M_PI);
    return difference;
}
