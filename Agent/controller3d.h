/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef CONTROLLER3D_H
#define CONTROLLER3D_H

#include "controller.h"
#include <vector>

class Controller3D : public Controller
{
public:
    bool initDone = false;

    double r_w;
    double r;
    double theta_w;

    //Fish school data
    vector<double> d_2D;
    vector<double> d_3D;
    vector<double> psi_arr;
    double v_av;

    double x_new, y_new, z_new;
    double phi_new, chi_new;
    double v_new, v_x_new, v_y_new, v_z_new;

    double noise_para, noise_perp, noise_z;

    int n_reject_z = 0;
    int n_reject_xy = 0;

    double renorm_K = 0.;

    Controller3D();

    void swim(double dt);

    void initFish();
    void act(double dt);
    void update();
    void reset();

    void initNoise(double dt);
    void computeFishSchoolData();

    /* --------------------------- */
    /* Function prototypes       */
    /* --------------------------- */
    double Friction(double v, vector<double> C_v);
    double Adapt(double v, vector<double> C_v);
    double f_w(double r_w, vector<double> C_f_w);
    double f_rot(double r_w, vector<double> C_f_rot);
    double g_w(double theta_w, vector<double> C_w);
    double g_rot(double theta_w, vector<double> C_rot);
    double f_att(double d, vector<double> C_f_att_d);
    double f_ali(double d, vector<double> C_f_ali_d);
    double E_att(double psi, vector<double> C_att);
    double E_ali(double psi, vector<double> C_ali);
    double Ep_att(double Dphi, vector<double> D_att);
    double Ep_ali(double Dphi, vector<double> D_ali);
    double Friction_z(double v_z, vector<double> C_v_z);
    double f_z(double z, vector<double> C_f_z);
    double f_att_z(double d_z, vector<double> C_f_att_z);
    double f_att_z_d(double d, vector<double> C_f_att_z_d);
    double E_att_z(double psi, vector<double> C_att_z);
    double Ep_att_z(double Dphi, vector<double> D_att_z);
    double generate_r_w(double x, double y, double z);
    double AngleModulo(double angle);
    double AngleModulo2(double angle);
    double normalizeAngle(double angle);
    double ran();
    double rand();
};

#endif // CONTROLLER3D_H
