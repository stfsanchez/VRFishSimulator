/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef DATAMANAGERCONSTANTS_H
#define DATAMANAGERCONSTANTS_H

#include <vector>
#include <map>
#include <string>

/* -------------------- */
/* PARAMETERS (macros)  */
/* -------------------- */
#define N_MAX             10
#define N_HIST_MAX        10000
#define N_PAR_MAX         20
#define NT_MAX            10000

/* For arrays with indices -N_HIST_MAX .. N_HIST_MAX, we allocate 2*N_HIST_MAX+1
   and use an offset */
#define OFFSET            (N_HIST_MAX)
#define ARRAY_SIZE        (2 * N_HIST_MAX + 1)

/* Distribution arrays (histograms) */
extern std::vector<double> dist_v;
extern std::vector<double> dist_dv;
extern std::vector<double> dist_r_w;
extern std::vector<double> dist_r;
extern std::vector<double> dist_theta_w;  /* index: k + OFFSET corresponds to Fortran index k */
extern std::vector<double> dist_d;
extern std::vector<double> dist_psi;
extern std::vector<double> dist_chi;
extern std::vector<double> dist_Dphi;
extern std::vector<double> dist_v_z;
extern std::vector<double> dist_z;
extern std::vector<double> dist_d_z;

/* Real fish distributions */
extern std::vector<double> dist_real_v;
extern std::vector<double> dist_real_dv;
extern std::vector<double> dist_real_r_w;
extern std::vector<double> dist_real_r;
extern std::vector<double> dist_real_theta_w;
extern std::vector<double> dist_real_d;
extern std::vector<double> dist_real_psi;
extern std::vector<double> dist_real_chi;
extern std::vector<double> dist_real_Dphi;
extern std::vector<double> dist_real_v_z;
extern std::vector<double> dist_real_z;
extern std::vector<double> dist_real_d_z;

//Size of the boxes for the different PDF; angle boxes of size Pi/n_angle
extern double dv;
extern double dv_z;
extern double v_z_min;
extern double dr_w;
extern double d_z;
extern double dd;
extern double d_z_min;
extern double dd_z;
extern double n_angle;
extern double dangle;

extern int symmetrize; //Symmetrize the angular PDF

extern std::map<int, std::vector<std::string>> trajectories;

extern int n_traj;             //Number of Saved trajectories
extern int t_traj;             //Duration of each trajectory
extern double dt_traj;            //Time step to save (multiple of dt)
extern int dt_interval_traj;    //Interval between starting times of different trajectories (> t_traj)

extern bool writeData;
extern bool writePictures;
extern bool writePDF;
extern bool writeTrajectories;
extern std::string dataFolder; //Folder where to store data

#endif // DATAMANAGERCONSTANTS_H
