/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "fish.h"
#include "controller.h"
#include "Tools/utils.h"

#include <iostream>
#include <sstream>


using namespace std;


Fish::Fish(double px, double py, double pz, double pphi, double pchi)
{
    x = px;
    y = py;
    z = pz;
    phi = pphi;
    chi = pchi;
    active = true;
}

void Fish::init(){
    if (this->ctrl != NULL){
        this->ctrl->initFish();
    }
    else{
        x = 0.0;
        y = 0.0;
        z = 0.0;
        phi = 0;
        chi = 0;
        active = true;
    }
}

void Fish::addPosition(Point &newPoint){
    positions.push_back(newPoint);
}

void Fish::addDirectionPhi(double newDir)
{
    phis.push_back(newDir);
}

void Fish::addDirectionChi(double newDir)
{
    chis.push_back(newDir);
}

void Fish::addNeighbors(vector<int> newNeighbors){
    neighbors_data.push_back(newNeighbors);
}

bool Fish::isNeighbor(int f){
    for (unsigned int i = 0; i<neighbors.size(); i++){
        if (f == neighbors[i]){
            return true;
        }
    }
    return false;
}

void Fish::act(double dt)
{
    active = ctrl->active;
    if (active){
        // Compute new coordinates and parameters
        ctrl->act(dt);
    }
}

void Fish::update(){
    active = ctrl->active;
    if (active){
        // Compute new coordinates and parameters
        ctrl->update();
    }
}

Fish::~Fish()
{
    positions.clear();
    phis.clear();
    chis.clear();
    neighbors.clear();
    neighbors_data.clear();
}

void Fish::reset(){
    x=0.0;
    y=0.0;
    z=0.0;
    phi = 0.0;
    chi = 0.0;
    neighbors.clear();
    positions.clear();
    phis.clear();
    chis.clear();
    neighbors_data.clear();
    active = true;
}

std::string Fish::toString(){
    std::stringstream ss;
    ss<<x/100.0<<" "<<y/100.0<<" "<<z/100.0<<" "<<phi<<" "<<chi;
    return ss.str();
}

std::string Fish::trajectoryData(){
    std::stringstream ss;
    ss<<x<<" "<<y<<" "<<z<<" "<<v_x<<" "<<v_y<<" "<<v_z;
    return ss.str();
}
