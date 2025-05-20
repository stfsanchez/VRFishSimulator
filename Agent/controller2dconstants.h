/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef CONTROLLER2DCONSTANTS_H
#define CONTROLLER2DCONSTANTS_H

// Model 2D controller constantss
extern double arena_radius;

extern int model_mode; //0: Guy, 1: Clement

extern double z0;
extern double zmean;

extern double tau0;
extern double taumean;
extern double taumin;

extern double vmean;
extern double vmin;
extern double vcut;

extern double vmem;
extern double vmem_12;

extern double gamma_rand; // au total =0.35 donc somme des racines carrees 0.3
extern double gamma_wall;
extern double gamma_attraction;
extern double gamma_alignement;
extern double gamma_sym;
extern double gamma_asym;
extern double L_ali;
extern double L_att; // 20 cm
extern double A;      // cm
extern double C;

extern double l_w;
extern double epsilon_2;

extern double gamma_r_chi;
extern double gamma_par_chi;  // pousser au maximum ( pas plus que 0.8)
extern double gamma_per_chi; // pousser 0 0,15 et 0.3 (eventuellement gros max 0.45)
extern double gamma_z;
extern double gamma_z_par;

extern double dw;
extern double dc;
extern double alpha_w;

extern int max_kick_computation_tentatives;

//Circle controller constants
extern double circle_zmean;
extern bool circle_bc;
extern double circle_radius;
extern bool circle_reverse;
extern double circle_v0;
extern double circle_tau0;
extern double circle_taumean;
extern double circle_taumin;
extern double circle_vmean;
extern double circle_vmin;
extern double circle_vcut;
extern int circle_referencefishid;
extern bool circle_fixedz;

//Rosace controller constants
extern double rosace_zmean;
extern double rosace_L; // Draw a rose of radius L (cm) and S-fold symmetry (S related to p and q)
extern double rosace_p;
extern double rosace_q;
extern bool rosace_reverse;
extern double rosace_v0;
extern double rosace_vmean;
extern bool rosace_fixedz;

//Fixed position controller constants
extern double fixed_zmean;
extern double fixed_distance;
extern double fixed_angle; //in degres
extern bool fixed_fixedz;
extern int fixed_referencefishid;

#endif // CONTROLLERCONSTANTS_H
