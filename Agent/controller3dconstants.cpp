/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "controller3dconstants.h"

std::string param_file = "";

int No_Int = 0;
int No_Wall  = 0;
int No_Wall_z = 0;

int K = -1;
int renorm_K_pow = 0;
int strategy = 1; // 0: K-Min ; 1: K-NN

//Parameters to control the different interactions
double Coef_Adapt      = 2.2;    //Speed adaptation
double Coef_Friction   = 1.0;
double Coef_Friction_z = 1.0;
double Coef_f_w        = 0.7;
double Coef_f_rot      = 1.0;
double Coef_f_z        = 1.0;
double Coef_f_att      = 1.8;
double Coef_f_ali      = 1.4;
double Coef_f_att_z    = 1.0;

//Noise parameters
double sigma_para = 3.3;
double sigma_perp = 2.8;
double sigma_Z    = 2.35;
double tau_XY     = 0.25;
double tau_Z      = 0.25;

//Other parameters
double d_c = 2.5;           // Effective distance between the coordinates of the fish and its head
double dBounce  = 0.15;     // Bounce > 1 after rejection by an angle 45° * (1 + dBounce * random)
double f_c = 2.;            // Intensity of the added short range repulsion
double d_c_att = 4.0;

double h_water = 14.73;
std::string geometry = "sphere"; // "sphere", "ellipsoid"

double Noise1      = 0;
double Noise2_para = 0;
double Noise2_perp = 0;
double Noise1_z = 0;
double Noise2_z = 0;

std::vector<double> C_v = std::vector<double>(N_PAR_V_MAX+1);
std::vector<double> C_f_w = std::vector<double>(N_PAR_F_W_MAX);         /* Fortran: 1-indexed = std::vector<double>(); here we use 0-indexed */
std::vector<double> C_f_rot = std::vector<double>(N_PAR_F_W_MAX);       /* same size as C_f_w */
std::vector<double> C_w = std::vector<double>(N_MODES_MAX);
std::vector<double> C_rot = std::vector<double>(N_MODES_MAX);
std::vector<double> C_f_att_d = std::vector<double>(N_PAR_D_MAX);
std::vector<double> C_f_ali_d = std::vector<double>(N_PAR_D_MAX);
std::vector<double> C_att = std::vector<double>(N_MODES_MAX);
std::vector<double> C_ali = std::vector<double>(N_MODES_MAX);
std::vector<double> D_att = std::vector<double>(N_MODES_MAX);
std::vector<double> D_ali = std::vector<double>(N_MODES_MAX);

std::vector<double> C_v_z = std::vector<double>(N_PAR_V_Z_MAX);
std::vector<double> C_f_z = std::vector<double>(N_PAR_Z_MAX+1);       /* Fortran: C_f_z(0:N_PAR_Z_MAX) */
std::vector<double> C_f_att_z = std::vector<double>(N_PAR_D_Z_MAX);
std::vector<double> C_f_att_z_d = std::vector<double>(N_PAR_Z_D_MAX);   /* Fortran: 1-indexed = std::vector<double>(); here 0-indexed */
std::vector<double> C_att_z = std::vector<double>(N_MODES_MAX);
std::vector<double> D_att_z = std::vector<double>(N_MODES_MAX);

double C_v_0 = -1.; //Speed control
double C_f_z_0 = -1.; //Average depth control
double C_f_att_d_1 = -1.; //Attraction range control

double v_cutoff = 100;

double epsilon = 0.00000001;
double epsilon2 = 0.0000000001;
