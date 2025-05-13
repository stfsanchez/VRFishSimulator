/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef FISH_H
#define FISH_H

#define ERROR 0.000001

#include "Tools/utils.h"
#include "controller.h"

using namespace std;

class Simulator;
class Controller;
class Fish{
public:
    // position, direction
    double x, y, z;
    double phi; // horizontal direction
    double chi; // vertical direction
    double v = 0.;
    double v_x = 0.;
    double v_y = 0.;
    double v_z = 0.;

    // physical parameters
    double length = 4.0; //0.03;
    double width = 0.6; //0.006;
    double collisionRadius = 0;
    double collisionRadiusSqrd = 0;

    // neighbors
    int k;
    vector<int> neighbors;

    int inSimID = -1; // ID of this Fish in the simulator
    Controller* ctrl = NULL; // Controller of this Fish

    bool active;

    //Save data
    vector<Point> positions;
    vector<double> phis;
    vector<double> chis;
    vector<vector<int>> neighbors_data;

    Fish(double px=0, double py=0, double pz= 0, double pphi=0, double pchi=0);
    ~Fish();

    void addPosition(Point &newPoint);
    void addDirectionPhi(double newDir);
    void addDirectionChi(double newDir);
    void addNeighbors(vector<int> newNeighbors);
    bool isNeighbor(int f);

    void init();

    void act(double dt);
    void update();

    void reset();

    inline void setID(int id) { inSimID = id; }
    inline void setController(Controller * nCtrl) { ctrl = nCtrl; ctrl->linkFish(this); }

    std::string toString();

    std::string trajectoryData();
};


#endif // FISH_H
