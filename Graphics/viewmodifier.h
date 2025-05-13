/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef VIEWMODIFIER_H
#define VIEWMODIFIER_H

#include "../Core/simulator.h"

class ViewModifier
{
public:

    int world_to_pixel;
    int window_center_x;
    int window_center_y;
    int window_width; //width
    int window_height; //height
    int camera_x;
    int camera_y;
    bool moving_camera = false;
    bool init_distrib_center = false;
    bool follow_fish = false;
    int focused_fish = -1;

    int offset_x = 0;
    int offset_y = 0;
    int offset_view_y = 110;
    int offset_view_y_2 = 80;

    Simulator * sim;

    double school_x;
    double school_y;
    double last_school_x;
    double last_school_y;
    double school_dir;
    double school_xmin;
    double school_xmax;
    double school_ymin;
    double school_ymax;
    double dx;
    double dy;

    double school_center_x;
    double school_center_y;
    double last_school_center_x;
    double last_school_center_y;
    double dxc;
    double dyc;

    double focused_fish_x;
    double focused_fish_y;
    double last_focused_fish_x;
    double last_focused_fish_y;

    ViewModifier();

    void setWindow(int x, int y, int width, int height);

    void setOffset(int offset_x, int offset_y);

    void setWorldToPixel(int value);

    void setMovingCamera(bool value);

    void setFollowFish(bool value, int fish_index);

    void setInitCenter(bool value);

    void setSimulator(Simulator* newSim);

    void computeCameraPosition();

    void reset();
};

#endif // VIEWMODIFIER_H
