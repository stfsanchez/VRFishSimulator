/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef CONTROLLER3DCONSTANTS_H
#define CONTROLLER3DCONSTANTS_H

#include <string>
#include <vector>

/* -------------------- */
/* PARAMETERS (macros)  */
/* -------------------- */

#define N_PAR_V_MAX       7
#define N_PAR_F_W_MAX     5
#define N_PAR_F_ROT_MAX   5
#define N_MODES_MAX       6
#define N_PAR_D_MAX       5

#define N_PAR_V_Z_MAX     5
#define N_PAR_Z_MAX       5
#define N_PAR_D_Z_MAX     5
#define N_PAR_Z_D_MAX     5

extern std::string param_file;

/* Noise coefficients */
extern double Noise1;
extern double Noise2_para;
extern double Noise2_perp;
extern double Noise1_z;
extern double Noise2_z;

/* Parameter arrays */
extern std::vector<double> C_v;         /* Fortran: C_v(0:N_PAR_V_MAX) */
extern std::vector<double> C_f_w;         /* Fortran: 1-indexed; here we use 0-indexed */
extern std::vector<double> C_f_rot;       /* same size as C_f_w */
extern std::vector<double> C_w;
extern std::vector<double> C_rot;
extern std::vector<double> C_f_att_d;
extern std::vector<double> C_f_ali_d;
extern std::vector<double> C_att;
extern std::vector<double> C_ali;
extern std::vector<double> D_att;
extern std::vector<double> D_ali;

extern std::vector<double> C_v_z;
extern std::vector<double> C_f_z;       /* Fortran: C_f_z(0:N_PAR_Z_MAX) */
extern std::vector<double> C_f_att_z;
extern std::vector<double> C_f_att_z_d;   /* Fortran: 1-indexed; here 0-indexed */
extern std::vector<double> C_att_z;
extern std::vector<double> D_att_z;

extern double C_v_0; //Speed control
extern double C_f_z_0; //Average depth control
extern double C_f_att_d_1; //Attraction range control

extern double v_cutoff;

extern int No_Int, No_Wall_z, No_Wall;
extern int K;
extern int renorm_K_pow;

/* --------------------------- */
/* Other global simulation   */
/* variables (from COMMON)   */
/* --------------------------- */
extern double Coef_Friction, Coef_Adapt, Coef_Friction_z;
extern double Coef_f_w, Coef_f_rot, Coef_f_z, Coef_f_att, Coef_f_ali, Coef_f_att_z;
extern double sigma_para, sigma_perp, sigma_Z, tau_XY, tau_Z;
extern double d_c, f_c, d_c_att;
extern double h_water, dBounce;
extern std::string geometry;
extern double epsilon;
extern double epsilon2;


#endif // CONTROLLER3DCONSTANTS_H
