/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "viewmodifier.h"

ViewModifier::ViewModifier()
{

}

void ViewModifier::setWindow(int x, int y, int width, int height){
    this->window_center_x = x;
    this->window_center_y = y;
    this->window_width = width;
    this->window_height = height;
    this->offset_x = 0;
    this->offset_y = 0;
}

void ViewModifier::setInitCenter(bool value){
    this->init_distrib_center = value;
}

void ViewModifier::setOffset(int offset_x, int offset_y){
    this->offset_x = offset_x;
    this->offset_y = offset_y;
}

void ViewModifier::setWorldToPixel(int value){
    this->world_to_pixel = value;
}

void ViewModifier::setSimulator(Simulator* newSim){
    this->sim = newSim;

    this->sim->computeFishData();
    this->sim->computeSchoolCenter();

    this->last_school_x = sim->school_x;
    this->school_x = sim->school_x;
    this->last_school_y = sim->school_y;
    this->school_y = sim->school_y;

    this->school_center_x = sim->school_center_x;
    this->school_center_y = sim->school_center_y;

    if (follow_fish && (focused_fish > -1 && focused_fish< (int)sim->fish.size())){
        this->focused_fish_x = this->sim->fish[focused_fish]->x;
        this->focused_fish_y = this->sim->fish[focused_fish]->y;
        last_focused_fish_x = focused_fish_x;
        last_focused_fish_y = focused_fish_y;
    }

    if (this->moving_camera){
        if (follow_fish && (focused_fish > -1 && focused_fish< (int)sim->fish.size())){
            this->camera_x = this->window_center_x - ( (int)(this->focused_fish_x) * this->world_to_pixel - this->window_width/2);
            this->camera_y = this->window_center_y - ( (int)(this->focused_fish_y) * this->world_to_pixel - this->window_height/2);
        }
        else if (init_distrib_center){
            this->camera_x = this->window_center_x - ( (int)(this->school_center_x) * this->world_to_pixel - this->window_width/2);
            this->camera_y = this->window_center_y - ( (int)(this->school_center_y) * this->world_to_pixel - this->window_height/2);
        }
        else{
            this->camera_x = this->window_center_x - ( (int)(this->school_x) * this->world_to_pixel - this->window_width/2);
            this->camera_y = this->window_center_y - ( (int)(this->school_y) * this->world_to_pixel - this->window_height/2);
        }

    }
    else{
        this->camera_x = window_center_x;
        this->camera_y = window_center_y;
    }
}

void ViewModifier::setMovingCamera(bool value){
    this->moving_camera = value;
}

void ViewModifier::setFollowFish(bool value, int fish_index){
    this->follow_fish = value;
    this->focused_fish = fish_index;
}

void ViewModifier::computeCameraPosition(){

    if (this->moving_camera)
    {        
        /*----- get data from simulator -------*/

        last_school_x = school_x;
        last_school_y = school_y;
        sim->computeFishData();
        school_x = sim->school_x;
        school_y = sim->school_y;


        if (follow_fish && (focused_fish>-1 && focused_fish< (int)this->sim->fish.size())){
            last_focused_fish_x = focused_fish_x;
            last_focused_fish_y = focused_fish_y;
            focused_fish_x = sim->fish[focused_fish]->x;
            focused_fish_y = sim->fish[focused_fish]->y;
            dx = focused_fish_x - last_focused_fish_x;
            dy = focused_fish_y - last_focused_fish_y;
        }
        else{
            dx = school_x - last_school_x;
            dy = school_y - last_school_y;
        }

        school_dir = sim->school_dir;
        school_xmin = sim->school_xmin;
        school_xmax = sim->school_xmax;
        school_ymin = sim->school_ymin;
        school_ymax = sim->school_ymax;

        sim->computeSchoolCenter();

        this->camera_x = this->camera_x - (int)dx * this->world_to_pixel;
        this->camera_y = this->camera_y - (int)dy * this->world_to_pixel;

        if (follow_fish && (focused_fish>-1 && focused_fish<(int)this->sim->fish.size())){
            this->offset_x = (int)(focused_fish_x)*world_to_pixel - (window_width/2) + camera_x - window_center_x;
            this->offset_y = (int)(focused_fish_y)*world_to_pixel - (window_height/2) + camera_y - window_center_y;
        }
        else if (this->init_distrib_center){
            this->school_center_x += dx;
            this->school_center_y += dy;
            this->offset_x = (int)(school_center_x)*world_to_pixel - (window_width/2) + camera_x - window_center_x;
            this->offset_y = (int)(school_center_y)*world_to_pixel - (window_height/2) + camera_y - window_center_y;
        }
        else{
            this->offset_x = (int)(school_x)*world_to_pixel - (window_width/2) + camera_x - window_center_x;
            this->offset_y = (int)(school_y)*world_to_pixel - (window_height/2) + camera_y - window_center_y;
        }
    }
    else{
        this->camera_x = window_center_x;
        this->camera_y = window_center_y;
        this->offset_x = - (window_width/2);
        this->offset_y = - (window_width/2);
    }

//    cout<<"VM @"<<sim->simSteps<<" dx, dy : "<<dx<<" | "<<dy
//        <<" dxc, dyc : "<<dxc<<" | "<<dyc
//        <<" camx, camy : "<<camera_x<<" | "<<camera_y
//        <<" offx, offy : "<<offset_x<<" | "<<offset_y
//        <<" school_x , school_y : "<<school_x<< " | "<<school_y
//        <<endl;
}

void ViewModifier::reset(){
    this->sim->computeFishData();
    this->sim->computeSchoolCenter();
    this->last_school_x = sim->school_x;
    this->school_x = sim->school_x;
    this->last_school_y = sim->school_y;
    this->school_y = sim->school_y;

    this->offset_x = 0;
    this->offset_y = 0;

    if (this->moving_camera){
        if (init_distrib_center){
            this->camera_x = this->window_center_x - (sim->school_center_x * this->world_to_pixel - this->window_width/2);
            this->camera_y = this->window_center_y - (sim->school_center_y * this->world_to_pixel - this->window_height/2);
        }
        else{
            this->camera_x = this->window_center_x - ((int)(sim->school_x) * this->world_to_pixel - this->window_width/2);
            this->camera_y = this->window_center_y - ((int)(sim->school_y) * this->world_to_pixel - this->window_height/2);
        }
    }
    else{
        this->camera_x = window_center_x;
        this->camera_y = window_center_y;
    }
}
