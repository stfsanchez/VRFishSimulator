/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "networkcontroller.h"

#include "fish.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

using namespace std;

NetworkController::NetworkController()
{
    type="network";
    onePassAction = true;
}

NetworkController::~NetworkController(){
    positions.clear();
    phis.clear();
    chis.clear();
}

void NetworkController::addPosition(Point &newPoint){
   positions.push_back(newPoint);
}

void NetworkController::addChi(double newDir)
{
   chis.push_back(newDir);
}

void NetworkController::addPhi(double newDir)
{
   phis.push_back(newDir);
}

void NetworkController::initFish(){
    if (positions.size()>0){
        Point p = this->positions.back();
        fish->x = p.x*100;
        fish->y = p.y*100;
        fish->z = p.z*100;
        fish->addPosition(p);

        double phi = this->phis.back();
        fish->phi = normalizeAngle(phi);
        fish->addDirectionPhi(phi);

        double chi = this->chis.back();
        fish->chi = normalizeAngle(chi);
        fish->addDirectionChi(chi);
    }
    else{
        Point p = Point(0,0,0);
        fish->x = p.x*100;
        fish->y = p.y*100;
        fish->z = p.z*100;
        fish->addPosition(p);

        double phi = 0;
        fish->phi = phi;
        fish->addDirectionPhi(phi);

        double chi = 0;
        fish->chi = chi;
        fish->addDirectionChi(chi);
    }

    active = true;
}

void NetworkController::act(double dt){
    if (positions.size()>0){
        Point p = this->positions.back();
        x_new = p.x*100;
        y_new = p.y*100;
        z_new = p.z*100;

        v_x_new = (x_new - fish->x) / dt;
        v_y_new = (y_new - fish->y) / dt;
        v_z_new = (z_new - fish->z) / dt;

        v_new =  sqrt(v_x_new*v_x_new + v_y_new*v_y_new);
    }

    if (phis.size()>0){
        double phi = phis.back();
        phi_new = normalizeAngle(phi);
    }

    if (chis.size()>0){
        double chi = chis.back();
        chi_new = normalizeAngle(chi);
    }

    active = true;
}

void NetworkController::update(){
    fish->x = x_new;
    fish->y = y_new;
    fish->z = z_new;
    fish->chi = chi_new;
    fish->phi = phi_new;
    fish->v_x = v_x_new;
    fish->v_y = v_y_new;
    fish->v_z = v_z_new;
    fish->v = v_new;

    Point p = Point(x_new, y_new, z_new);
    fish->addPosition(p);
    fish->addDirectionPhi(phi_new);
    fish->addDirectionChi(chi_new);
}

void NetworkController::reset(){
    initFish();
}

double NetworkController::normalizeAngle(double angle) {
    angle = std::fmod(angle + M_PI, 2 * M_PI);
    if (angle < 0)
        angle += 2 * M_PI;
    return angle - M_PI;
}
