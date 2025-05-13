/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "simulator.h"

#include "../Agent/controller.h"
#include "globalconstants.h"
#include <cmath>
#include <iostream>
#include <map>

using namespace std;

Simulator::Simulator()
{
    fish = vector<Fish*>();
    currentFishID = 0;
    initTime = 0.0;
    actualTime = 0.0;
    previousTime = 0.0;
    dt = DT;
    simSteps = 0;

    activeIndex = 0;
    nbIcpt = 0;

    pause = true;
}

Simulator::Simulator(double initTime)
{
    fish = vector<Fish*>();
    currentFishID = 0;
    this->initTime = initTime;
    this->actualTime = this->initTime;
    this->previousTime = this->actualTime;
    simSteps = 0;

    activeIndex = 0;
    nbIcpt = 0;

    pause = true;
}


Simulator::~Simulator()
{
    for (unsigned int i=0; i < fish.size(); i++) {
        delete fish[i];
    }
}



void Simulator::addFish(Fish * f)
{
    fish.push_back(f);
    f->setID(currentFishID);
    currentFishID++;
    if (f->ctrl != NULL){
        f->ctrl->linkSimulator(this);
    }
}


void Simulator::addTank(FishTank *ft)
{
    tank = ft;
}

void Simulator::oneStep()
{
    if (!pause)
    {
        //cout << " -- Step " << simTime << " -- " << endl;
        simSteps++;

        if (!NETWORK_TIME){
            this->actualTime += this->dt;
        }

        std::vector<int> delayedUpdates;
        delayedUpdates.clear();

        for (unsigned int i=0; i < fish.size(); i++) {
            fish[i]->act(this->dt);
            if (fish[i]->ctrl->onePassAction) {
                fish[i]->update();
            }
            else{
                delayedUpdates.push_back(i);
            }
        }

        for (unsigned int j=0; j<delayedUpdates.size(); j++){
            fish[delayedUpdates[j]]->update();
        }
    }
}

void Simulator::setActualTime(float pTime){
    //First time !
    if (this->simSteps<1){
        this->initTime = pTime;
        this->previousTime = pTime;
        this->actualTime = pTime;
        this->dt = this->actualTime - this->previousTime;
    }
    else{
        this->previousTime = this->actualTime;
        this->actualTime = pTime;
        this->dt = this->actualTime - this->previousTime;
    }
}

void Simulator::setDt(double pDt){
    this->dt = pDt;
}

bool Simulator::simulationFinished()
{
    if (this->actualTime > (SIM_TIME + T_MIN)){
        return true;
    }

    for(unsigned int i = 0; i < fish.size(); i++){
        if (fish[i]->active) {
            return false;
        }
    }

    cout<<"End of sim !"<<endl;
    return true;
}

void Simulator::reset(){
    for (unsigned int i=0; i<fish.size(); i++){
        fish[i]->ctrl->reset();
    }
    this->actualTime = this->initTime;
    this->previousTime = this->actualTime;
    simSteps = 0;

    activeIndex = 0;
    nbIcpt = 0;

    computeFishData();
}

void Simulator::computeFishData(){
    last_school_x = school_x;
    last_school_y = school_y;

    school_x = fish[0]->x;
    school_y = fish[0]->y;
    school_dir = fish[0]->phi;
    school_xmin = fish[0]->x;
    school_xmax = fish[0]->x;
    school_ymin = fish[0]->y;
    school_ymax = fish[0]->y;

    for (unsigned int i=1; i<fish.size(); i++){
        school_x += fish[i]->x;
        school_y += fish[i]->y;
        school_dir += fish[i]->phi;

        school_xmin = std::min(school_xmin, fish[i]->x);
        school_xmax = std::max(school_xmax, fish[i]->x);

        school_ymin = std::min(school_ymin, fish[i]->y);
        school_ymax = std::max(school_ymax, fish[i]->y);
    }

    school_x = school_x / (double)fish.size();
    school_y = school_y / (double)fish.size();
    school_dir = school_dir / (double)fish.size();

    if (simSteps==0){
        last_school_x = school_x;
        last_school_y = school_y;
    }

/*
    cout << " -- School : center " << school_x << " , " << school_y
         << " | X " << school_xmin << " , " << school_xmax
         << " | Y " << school_ymin << " , " << school_ymax
         << " | dir " << school_dir <<endl;
*/
}

void Simulator::computeSchoolCenter(){
    int last_school_center_x = school_center_x;
    int last_school_center_y = school_center_y;

    std::map<int, int> distrib_X;
    std::map<int, int> distrib_Y;

    std::map<int, int>::iterator itx;
    int x;
    std::map<int, int>::iterator ity;
    int y;
    for (unsigned int i=0; i<fish.size(); i++){
         // X
        x = (int)(std::floor(fish[i]->x));
         itx = distrib_X.find(x);
         if (itx != distrib_X.end()){
            distrib_X[x]++;
         }
         else {
            //distrib_X.insert(std::pair<int,int>(x,1));
             distrib_X[x] = 1;
         }
         // Y
         y = (int)(std::floor(fish[i]->y));
         ity = distrib_Y.find(y);
         if (ity != distrib_Y.end()){
            distrib_Y[y]++;
         }
         else {
            //distrib_Y.insert(std::pair<int,int>(y,1));
            distrib_Y[y] = 1;
         }
    }

    itx = distrib_X.begin();
    school_center_x = itx->first;
    itx++;
    while (itx != distrib_X.end()){
        if (itx->second > distrib_X[school_center_x]){
            school_center_x = itx->first;
        }
        itx++;
    }
    ity = distrib_Y.begin();
    school_center_y = ity->first;
    ity++;
    while (ity != distrib_Y.end()){
        if (ity->second > distrib_Y[school_center_y]){
            school_center_y = ity->first;
        }
        ity++;
    }

    if (simSteps>0){
        if (distrib_X[last_school_center_x]==distrib_X[school_center_x]){
            school_center_x = last_school_center_x;
        }

        if (distrib_Y[last_school_center_y]==distrib_Y[school_center_y]){
            school_center_y = last_school_center_y;
        }
    }
}
