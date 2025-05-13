/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "datamanagerconstants.h"

/* Distribution arrays (histograms) */
std::vector<double> dist_v = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_dv = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_r_w = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_r = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_theta_w = std::vector<double>(ARRAY_SIZE);  /* index: k + OFFSET corresponds to Fortran index k */
std::vector<double> dist_d = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_psi = std::vector<double>(ARRAY_SIZE);
std::vector<double> dist_chi = std::vector<double>(ARRAY_SIZE);
std::vector<double> dist_Dphi = std::vector<double>(ARRAY_SIZE);
std::vector<double> dist_v_z = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_z = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_d_z = std::vector<double>(N_HIST_MAX + 1);

/* real fish distributions */
std::vector<double> dist_real_v = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_real_dv = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_real_r_w = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_real_r = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_real_theta_w = std::vector<double>(ARRAY_SIZE);
std::vector<double> dist_real_d = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_real_psi = std::vector<double>(ARRAY_SIZE);
std::vector<double> dist_real_chi = std::vector<double>(ARRAY_SIZE);
std::vector<double> dist_real_Dphi = std::vector<double>(ARRAY_SIZE);
std::vector<double> dist_real_v_z = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_real_z = std::vector<double>(N_HIST_MAX + 1);
std::vector<double> dist_real_d_z = std::vector<double>(N_HIST_MAX + 1);


//Size of the boxes for the different PDF; angle boxes of size Pi/n_angle
double dv      = 0.4;
double dv_z    = 0.16;
double v_z_min = -8.0;
double dr_w    = 0.5;
double d_z     = 0.2;
double dd      = 0.5;
double d_z_min = -8.0;
double dd_z    = 0.16;
double n_angle = 45.;
double dangle = 0.;

int symmetrize = 1;

std::map<int, std::vector<std::string>> trajectories = std::map<int, std::vector<std::string>>();

int n_traj = 3;                //Number of Saved trajectories
int t_traj   = 90;             //Duration of each trajectory
double dt_traj  = 0.1;         //Time step to save (multiple of dt)
int dt_interval_traj = 200;    //Interval between starting times of different trajectories (> t_traj)

bool writeData = false;
bool writePictures = false;
bool writePDF = false;
bool writeTrajectories = false;
std::string dataFolder = "./Data/";
