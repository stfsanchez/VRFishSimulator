/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "replaycontroller.h"
#include "fish.h"
#include <cmath>
#include <iostream>
#include "globalconstants.h"

using namespace std;


ReplayController::ReplayController()
{
    type="replay";
    onePassAction = true;
}

ReplayController::~ReplayController(){
    positions.clear();
    directions.clear();
    neighbors.clear();
}

void ReplayController::addPosition(Point &newPoint){
   positions.push_back(newPoint);
}

void ReplayController::addDirection(double newDir)
{
   directions.push_back(newDir);
}

void ReplayController::addNeighbors(vector<int> newNeighbors){
    neighbors.push_back(newNeighbors);
}

void ReplayController::initFish(){
    replayIndex = 0;
    Point p = positions[replayIndex];
    fish->x = p.x*100;
    fish->y = p.y*100;
    fish->addPosition(p);

    double d = directions[replayIndex];
    fish->phi = d;
    fish->addDirectionPhi(d);

    if (neighbors.size()>0){
        bool newKick = false;
        vector<int> n = neighbors[replayIndex];
        for (int i=0; i<fish->k; i++){
            if (n[i] != -1){
                newKick = true;
            }
        }
        if (newKick){
            neighbors_frame = 0;
            fish->neighbors = n;
            fish->addNeighbors(n);
        }
        else{
            if (neighbors_frame < NEIGHBORS_MAX_FRAMES){
                fish->addNeighbors(fish->neighbors);
                neighbors_frame++;
            }
            else{
                neighbors_frame = 0;
                fish->neighbors = n;
                fish->addNeighbors(n);
            }
        }
    }

    replayIndex ++;

    active = replayIndex<positions.size();
}

void ReplayController::act(double dt){
    Point p = positions[replayIndex];
    fish->x = p.x*100;
    fish->y = p.y*100;
    fish->addPosition(p);

    double d = directions[replayIndex];
    fish->phi = d;
    fish->addDirectionPhi(d);

    if (neighbors.size()>0){
        bool newKick = false;
        vector<int> n = neighbors[replayIndex];
        for (int i=0; i<fish->k; i++){
            if (n[i] != -1){
                newKick = true;
            }
        }
        if (newKick){
            neighbors_frame = 0;
            fish->neighbors = n;
            fish->addNeighbors(n);
        }
        else{
            if (neighbors_frame < NEIGHBORS_MAX_FRAMES){
                fish->addNeighbors(fish->neighbors);
                neighbors_frame++;
            }
            else{
                neighbors_frame = 0;
                fish->neighbors = n;
                fish->addNeighbors(n);
            }
        }
    }

    replayIndex ++;

    active = replayIndex<positions.size();
}

void ReplayController::update(){

}

void ReplayController::reset(){
    active = true;
    replayIndex = 0;
    neighbors_frame = 0;
    fish->reset();
    fish->x = positions[0].x*100;
    fish->y = positions[0].y*100;
    fish->phi = directions[0];
}
