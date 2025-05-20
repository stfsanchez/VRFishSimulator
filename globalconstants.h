/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

#include <string>

//--- Graphics data ---//
extern bool GRAPHIC_MODE; // Graphic mode
//Window geometry, size of pictures (in pixel)
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int WORLD_TO_PIXEL;  //Scaling factor - conversion from meter to pixel - Zoom in or out (lesser is out)
extern bool ANTIALIASING;   //Use antialiasing to render pictures (true = antialiasing on)
extern bool MOVING_CAMERA; //Indicates if the camera follows school barycenter (true) or is fixed (false)
extern bool INIT_CAMERA_AT_DISTRIB_CENTER; //Initialization at school distribution center
extern bool FOLLOW_FISH; //Camera is following focused fish (true)
extern int FOCUSED_FISH; //Index of focused fish
extern bool BACKGROUND; //Indicates if there is a background
extern bool DARK_BACKGROUND; //Dark background on (true)
extern bool GRID; //Indicates if there is a grid to be displayed
extern double GRID_SQUARE_SIZE; //Grid square size in real world coordinates (in cm)
extern bool WHITE_FISH; //Fish in white (true) instead of colors (false)
extern bool SHOW_TRAJECTORY; //Show fish trajectory (true)
extern bool SHOW_FISH; //Show fish shape (true)
extern int TRAJECTORY_LENGTH; //Trajectory length
extern int TRAJECTORY_WIDTH; //Trajectory width (in pixel)

//--- Sim data ---//
extern double DT; //Simulator delta time between step in seconds
extern int SIM_TIME; //Simulation time while not in graphic mode
extern int T_MIN;
extern bool NETWORK_TIME; //Simulator time is network controlled
extern bool FISHTANK; //Indicates if there is a fishtank
extern int NB_FISH_REPLAY;        // Number of fish with replay controller
extern int NB_FISH_CIRCLE;        // Number of fish controlled with circle behavior
extern int NB_FISH_ROSACE;        // Number of fish controlled with rosace behavior
extern int NB_FISH_FIXED;         // Number of fish controlled with fixed position behavior
extern int NB_FISH_2D_MODEL;         // Number of fish controlled with 2D model
extern int NB_FISH_3D_MODEL;         // Number of fish controlled with 3D model
extern int NB_FISH_NETWORK;       // Number of fish with network controller
//extern int DIMS; // Number of dimensions (2 for 2D, 3 for 3D)
extern int NB_FISH; //Total number of fish
//extern std::string CONTROL_MODE; // controller type/mode to control fish

extern std::string REPLAY_FILE; // File to read for replaying data
extern std::string IP_IN; //To connect network controller
extern int PORT_IN; //To connect network controller
extern std::string IP_OUT; //To connect network controller
extern int PORT_OUT; //To connect network controller
extern std::string WORLD_FILE;
extern int FRAME_ID;


//--- Influencial Neighbors ---//
extern std::string NEIGHBORS_FILE; //File to read neighbors data
extern bool SHOW_NEIGHBORS; //True : influencial neighbors on
extern int K_NEIGHBORS; //Number of influencial neighbors
//extern std::vector<int> NEIGHBORS_FOCAL_FISH; //Array of index of focal fish to show neighbors
extern int NEIGHBORS_FOCAL_FISH; // Index of focal fish to show neighbors
extern int NEIGHBORS_MAX_FRAMES; // Max consecutive frames to show neigbors
extern bool FIX_NEIGHBORS_INDEXES; // Put indexes from {1, N} to {0, N-1}

#endif // GLOBALCONSTANTS_H
