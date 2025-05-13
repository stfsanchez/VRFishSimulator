/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "rosacecontroller.h"

#include "controller.h"
#include "../Core/simulator.h"
#include "controller2dconstants.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

RosaceController::RosaceController() {
    type = "rosace";
    this->initDone = false;
    this->onePassAction = true;
}

void RosaceController::initFish(){
    this->K = rosace_p / rosace_q;
    this->radius = rosace_L;
    this->fixedz = rosace_fixedz;

    this->reverse = rosace_reverse;

    if (this->reverse){
        this->newspeed = -1.*rosace_v0;
        this->newtheta = M_PI;
        this->th = M_PI;
    }
    else{
        this->newspeed = rosace_v0;
        this->newtheta = 0.;
        this->th = 0;
    }

    this->newx = this->radius*cos(this->newtheta);
    this->newy = this->radius*sin(this->newtheta);

    if (this->fixedz){
        this->newz = rosace_zmean;
    }
    else{
        this->newz = 0.;
    }

    if (this->reverse){
        this->newphi = this->newtheta - M_PI/2.; //angle_to_pipi(this->newtheta + M_PI/2);
    }
    else{
        this->newphi = this->newtheta + M_PI/2.; //angle_to_pipi(this->newtheta + M_PI/2);
    }

    this->newchi = 0;

    this->fish->active = true;

    this->initDone = true;

    this->update();
}

void RosaceController::swim(){
    double dt = this->sim->dt;

    this->omega = (this->speed/1.) / this->radius / sqrt((this->K*this->K + 1)/2.); // (cm/s)

    this->newspeed = this->speed;
    this->newtheta = this->theta + this->omega*dt;

    this->newx = this->radius*cos(this->K*this->newtheta)*cos(this->newtheta); //cm !
    this->newy = this->radius*cos(this->K*this->newtheta)*sin(this->newtheta); //cm !

    if (this->fixedz){
        this->newz = rosace_zmean;
    }

    double dx = this->newx - this->fish->x;
    double dy = this->newy - this->fish->y;
    this->newphi = atan2(dy,dx);

    this->newchi = 0;
}


void RosaceController::act(double dt){
    if (!this->initDone){
        this->initFish();
    }
    else{
        this->num_steps ++;
        this->swim();
    }
}

void RosaceController::update(){
    this->theta = this->newtheta;
    this->speed = this->newspeed;

    this->fish->x = this->newx;
    this->fish->y = this->newy;
    this->fish->z = this->newz;
    this->fish->chi = this->newchi;
    this->fish->phi = this->newphi;
    this->fish->v = this->newspeed;
}

void RosaceController::reset(){
    this->initDone = false;
}

void RosaceController::setReverse(bool pValue){
    rosace_reverse = pValue;
    this->reverse = rosace_reverse;

    if (this->reverse){
        this->speed = -1.*rosace_v0;
        this->newtheta = this->theta + M_PI;
    }
    else{
        this->speed = rosace_v0;
        this->newtheta = this->theta - M_PI;
    }
}

void RosaceController::setMeanSpeed(double pSpeed){
    rosace_v0 = pSpeed;
    if (this->reverse){
        this->speed = -1.*rosace_v0;
    }
    else{
        this->speed = rosace_v0;
    }
}
