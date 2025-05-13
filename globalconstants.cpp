/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "globalconstants.h"

//------- Main settings --------

double DT = 0.1; //Simulator delta time between step in seconds
bool NETWORK_TIME = false;
int SIM_TIME = 3600; //seconds
int T_MIN = 120; //Minimum time before recording data - necessary to stabilize fish behaviors

int NB_FISH_REPLAY = 0;        // Number of fish
int NB_FISH_2D_MODEL = 0;
int NB_FISH_3D_MODEL = 0;
int NB_FISH_CIRCLE = 0;
int NB_FISH_ROSACE = 0;
int NB_FISH_FIXED = 0;         // Number of fish controlled with fixed position behavior
int NB_FISH_NETWORK = 0;
int NB_FISH = 0; //Total number of fish

//int DIMS = 2; // Number of dimensions (2 for 2D, 3 for 3D)

//------- Replay controller --------
std::string REPLAY_FILE = ""; // File to read for replaying data

//------- Network controller -------
std::string IP_IN = "127.0.0.1"; //To connect network controller
int PORT_IN = 65432; //To connect network controller

//------- UDP writer
std::string IP_OUT = "127.0.0.1"; //To connect network controller
int PORT_OUT = 65435; //To connect network controller
std::string WORLD_FILE = "foo";
int FRAME_ID = -1;

//------- Display settings --------

bool GRAPHIC_MODE = true; // Graphic mode

//Window geometry, size of pictures
int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 500;

int WORLD_TO_PIXEL = 8;  //Scaling factor - conversion from meter to pixel - Zoom in or out (lesser is out)

bool ANTIALIASING = true;   //Use antialiasing to render pictures (true = antialiasing on)

bool GRID = false; //Indicates if there is a grid to be displayed
double GRID_SQUARE_SIZE = 20; //Grid square size in real world coordinates (in cm)

bool BACKGROUND = false; //Indicates if there is a background
bool DARK_BACKGROUND = false; //Dark background on (true)

bool FISHTANK = true; //Indicates if there is a fishtank

bool SHOW_FISH = true; //Show fish shape (true)
bool WHITE_FISH = true; //Fish in white (true) instead of colors (false)

bool SHOW_TRAJECTORY = false; //Show fish trajectory (true)
int TRAJECTORY_LENGTH = 10; //Trajectory length
int TRAJECTORY_WIDTH = 1; //Trajectory width (in pixel)

//------- Camera settings --------

bool MOVING_CAMERA = false; //Indicates if the camera follows school barycenter (true) or is fixed (false)
bool INIT_CAMERA_AT_DISTRIB_CENTER = false; //Change barycenter (false) to distribution discrete center (true)
bool FOLLOW_FISH = false; //Camera is following focused fish (true)
int FOCUSED_FISH = -1; //Index of focused fish

//------- Influencial Neighbors Display settings --------

bool SHOW_NEIGHBORS = false; //True : influencial neighbors on

int K_NEIGHBORS = 0; //Number of influencial neighbors

std::string NEIGHBORS_FILE= ""; //Neighbors data file

int NEIGHBORS_FOCAL_FISH = 0; //Index of focal fish to show neighbors

int NEIGHBORS_MAX_FRAMES = 5; // Max consecutive frames to show neigbors

bool FIX_NEIGHBORS_INDEXES = false; // Put indexes from {1, N} to {0, N-1}

//std::vector<int> NEIGHBORS_FOCAL_FISH; //Index of focal fish to show neighbors


