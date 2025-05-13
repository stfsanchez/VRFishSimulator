/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "circularcontroller.h"

#include "controller.h"
#include "../Core/simulator.h"
#include "controller2dconstants.h"
#include "Tools/rngsingleton.h"


#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

using namespace std;

CircularController::CircularController()
{
    type = "circular";
    this->initDone = false;
    this->onePassAction = true;
}

void CircularController::initFish(){
    this->num_steps = 0;
    this->num_kick = 0;

    this->burstandcoast= circle_bc;
    this->radius = circle_radius;
    this->fixedz = circle_fixedz;
    this->referencefishid = circle_referencefishid;

    this->reverse = circle_reverse;

    if (burstandcoast){
        this->kicktime = this->sim->actualTime;

        this->kickspeed = circle_vmean;

        if (this->reverse){
            this->kicktheta = M_PI;
        }
        else{
             this->kicktheta = 0.;
        }
        double dtau = circle_taumean - circle_taumin;
        this->newtau = circle_taumin - dtau * 0.5 * log(rndm()*rndm());
        this->kicking = true;

        this->newspeed = this->kickspeed;
        this->newtheta = this->kicktheta;
    }
    else{
        if (this->reverse){
            this->newspeed = -1.*circle_v0;
            this->newtheta = M_PI;
        }
        else{
            this->newspeed = circle_v0;
            this->newtheta = 0.;
        }
    }

    this->newx = this->radius*std::cos(this->newtheta);
    this->newy = this->radius*std::sin(this->newtheta);
    if (this->fixedz){
        this->newz = circle_zmean;
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

void CircularController::swim(){
    double dt = this->sim->dt;

    this->omega = (this->speed/1.) / this->radius; // (cm/s)

    this->newspeed = this->speed;
    this->newtheta = this->theta + this->omega*dt;

    this->newx = (this->radius*std::cos(this->newtheta)); //cm !
    this->newy = (this->radius*std::sin(this->newtheta)); //cm !

    if (this->fixedz){
        this->newz = circle_zmean;
    }
    else{
        this->newz = this->sim->fish[this->referencefishid]->z;
    }

    if (this->reverse){
        this->newphi = this->newtheta - M_PI/2.; //angle_to_pipi(this->newtheta + M_PI/2);
    }
    else{
        this->newphi = this->newtheta + M_PI/2.; //angle_to_pipi(this->newtheta + M_PI/2);
    }
    this->newchi = 0;
}

void CircularController::glide(){
    double dt = this->sim->actualTime - this->kicktime;
    double expt = std::exp(-dt/circle_tau0);

    this->newspeed = this->kickspeed * expt;
    this->omega = (this->newspeed/1.) / this->radius; // (m/s)
    //this->newtheta = this->kicktheta + this->omega*dt;

    if (reverse){
        this->newtheta = this->theta - this->omega*this->sim->dt;

    }
    else{
        this->newtheta = this->theta + this->omega*this->sim->dt;

    }

    this->newx = 1.* (this->radius*std::cos(this->newtheta)); //cm !
    this->newy = 1.* (this->radius*std::sin(this->newtheta)); //cm !

    if (this->fixedz){
        this->newz = circle_zmean;
    }
    else{
        this->newz = this->sim->fish[this->referencefishid]->z;
    }

    if (this->reverse){
        this->newphi = this->newtheta - M_PI/2.; //angle_to_pipi(this->newtheta + M_PI/2);
    }
    else{
        this->newphi = this->newtheta + M_PI/2.; //angle_to_pipi(this->newtheta + M_PI/2);
    }
    this->newchi = 0;
}

bool CircularController::checkKick(){
    double bcDuration = this->tau;
    double timeSpent = this->sim->actualTime - this->kicktime;
    bool test = timeSpent > bcDuration || this->fish->v < circle_vmin;
    return test;
}

void CircularController::kick(){
    //Kick !
    this->num_kick++;
    this->kicktime = this->sim->actualTime;
    this->kicktheta = this->theta;

    do{
        this->kickspeed = circle_vmin + circle_vmean * (-log(rndm()*rndm()*rndm())) / 3.;
    } while (this->kickspeed > circle_vcut);

    this->newspeed = this->kickspeed;

    double dtau = circle_taumean - circle_taumin;
    this->newtau = circle_taumin - dtau * 0.5 * log(rndm()*rndm());
}

void CircularController::act(double dt){
    if (!this->initDone){
        this->initFish();
    }
    else{
        this->num_steps ++;
        if (this->burstandcoast){
            this->glide();
            this->kicking = this->checkKick();
            if (this->kicking){
                this->kick();
            }
        }
        else{
            this->swim();
        }
    }
}

void CircularController::update(){
    this->theta = this->newtheta;
    this->speed = this->newspeed;

    this->fish->x = this->newx;
    this->fish->y = this->newy;
    this->fish->z = this->newz;
    this->fish->chi = this->newchi;
    this->fish->phi = this->newphi;
    this->fish->v = this->newspeed;

    if (this->burstandcoast && this->kicking){
        this->tau = this->newtau;
        this->kicking = false;
    }
}

void CircularController::reset(){
    this->initDone = false;
}


double CircularController::rndm(){
    RNGSingleton& rng = RNGSingleton::getInstance();
    return rng.getRandomNumber();
}

double CircularController::angle_to_pipi(double difference) const
{
    do {
        if (difference < -M_PI)
            difference += 2. * M_PI;
        if (difference > M_PI)
            difference -= 2. * M_PI;
    } while (std::abs(difference) > M_PI);
    return difference;
}

void CircularController::setReverse(bool pValue){
    circle_reverse = pValue;
    this->reverse = circle_reverse;
    if (this->burstandcoast){
        if (this->reverse){
            this->newtheta = this->theta + M_PI;
        }
        else{
            this->newtheta = this->theta - M_PI;
        }
    }
    else{
        if (this->reverse){
            this->speed = -1.*circle_v0;
            this->newtheta = this->theta + M_PI;
        }
        else{
            this->speed = circle_v0;
            this->newtheta = this->theta - M_PI;
        }
    }
}

void CircularController::setMeanSpeed(double pSpeed){
    if (this->burstandcoast){
        circle_vmean = pSpeed;
    }
    else{
        circle_v0 = pSpeed;
        if (this->reverse){
            this->speed = -1.*circle_v0;
        }
        else{
            this->speed = circle_v0;
        }
    }
}
