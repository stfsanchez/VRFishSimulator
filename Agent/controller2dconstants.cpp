/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "controller2dconstants.h"

double arena_radius = 26.0;

int model_mode = 0; //0: Guy, 1: Clement

double z0 = 7.0;
double zmean = 12.0;

double tau0 = 0.8; //0.8;
double taumean = 0.825; // tau moyen
double taumin = 0.2;

double vmean = 43.; //8.; //43
double vmin = 1.;
double vcut = 15.; //7.; //35

double vmem = 0.9;
double vmem_12 = 5.;

double gamma_rand = 0.3; // au total =0.35 donc somme des racines carees 0.3
double gamma_wall = 0.23;
double gamma_attraction = 0.3;
double gamma_alignement = 0.3;
double gamma_sym = 0.;
double gamma_asym = 0.;
double L_ali = 20;
double L_att = 20; // 20 cm
double A = 3;      // cm
double C = 0.4;

double gamma_r_chi = 0.15;
double gamma_par_chi = 0.2;  // pousser au maximum ( pas plus que 0.8)
double gamma_per_chi = 0.15; // pousser 0 0,15 et 0.3 (eventuellement gros max 0.45)
double gamma_z = 0;
double gamma_z_par = 0;

double l_w = 0.6;
double epsilon_2 = 0.7;

double dw = 6.;
double dc = 1.;
double alpha_w = 0.;

int max_kick_computation_tentatives = 500;

//Circle controller constants
double circle_zmean = 6.0;
bool circle_bc = false;
double circle_radius = 0.20;
bool circle_reverse = false;
double circle_v0 = 10.;
double circle_tau0 = 0.8;
double circle_taumean = 0.825;
double circle_taumin = 0.2;
double circle_vmean = 43.;
double circle_vmin = 1.;
double circle_vcut = 35.;
int circle_referencefishid = 0;
bool circle_fixedz = true;

//Rosace controller constants
double rosace_zmean = 5.;
double rosace_L = 20.; // Draw a rose of radius L (cm) and S-fold symmetry (S related to p and q)
double rosace_p = 5.;
double rosace_q = 2.;
double rosace_v0 = 8.;
double rosace_vmean = 8.;
bool rosace_fixedz = true;
bool rosace_reverse = false;


//Fixed position controller constants
double fixed_zmean = 6.;
double fixed_distance = 5.;
double fixed_angle = 45.; //in degres
int fixed_referencefishid = 0;
bool fixed_fixedz = true;
