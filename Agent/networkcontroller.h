/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include "controller.h"
#include "Tools/utils.h"

class NetworkController : public Controller
{
public:
    double x_new, y_new, z_new;
    double phi_new, chi_new;
    double v_new, v_x_new, v_y_new, v_z_new;

    vector<Point> positions;
    vector<double> chis;
    vector<double> phis;

    NetworkController();
    ~NetworkController();

    void addPosition(Point &newPoint);
    void addPhi(double newDir); //horizontal
    void addChi(double newDir); //Vertical

    void initFish();
    void act(double dt);
    void update();
    void reset();

    double normalizeAngle(double angle);
};

#endif // NETWORKCONTROLLER_H
