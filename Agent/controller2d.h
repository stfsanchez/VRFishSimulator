/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef CONTROLLER2D_H
#define CONTROLLER2D_H

#include "controller.h"
#include <vector>

using namespace std;

class Controller2D : public Controller
{
public:
    int model_mode = 0;

    bool initDone;
    int phase; //0: burst, 1: coast

    bool kicking;
    int num_steps;
    int num_kick;

    //Fish data
    double tau;
    double kicktime;
    double kickspeed;
    double kickx;
    double kicky;
    double kickz;

    double newx, newy, newz;
    double newphi, newchi;
    double newtau;
    double newspeed;
    double dl_next;

    //Interactions
    double fw = 0.;
    double dphi_wall;
    double dphi_fish;
    //z !
    double dchi_fish;
    double dchi_par;
    double dchi_per;

    //Neighbors
    vector<int> neighbors; //fish indexes in simulator fish vector

    Controller2D();

    double interaction_wall();
    void computeNeighbors();
    double interaction_fish(Fish* neighbor);
    double d_phi_random(double g_number, double gamma_random);
    double angle_vue(Fish* neighbor);
    double distance_entre_poissons(Fish* neighbor);
    double coast_length(double initial_speed, double reference_time, double time);
    double chi_par(double gamma, double chi);

    void glide();
    void kick();
    bool checkKick();
    void computeInteractions();

    void initFish();
    void act(double dt);
    void update();
    void reset();

    double rndm();
    double angle_to_pipi(double difference) const;
};

#endif // CONTROLLER2D_H
