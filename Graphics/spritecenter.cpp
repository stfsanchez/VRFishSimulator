/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "spritecenter.h"

#include <iostream>

SpriteCenter::SpriteCenter(Simulator* sim)
{
    this->sim = sim;
}

void SpriteCenter::setDistributionCenter(bool value){
    this->distribution_center = value;
}

void SpriteCenter::setColor(QColor color){
    this->color = color;
}

void SpriteCenter::draw(QPainter*  painter, ViewModifier* vm, int frame){
    painter->setBrush(this->color);

    if (this->distribution_center){
       x = vm->school_center_x*vm->world_to_pixel - (vm->window_width/2) + vm->camera_x - vm->offset_x;
       y = vm->school_center_y*vm->world_to_pixel - (vm->window_height/2) + vm->camera_y - vm->offset_y - vm->offset_view_y;
    }
    else{
       x = sim->school_x*vm->world_to_pixel - (vm->window_width/2) + vm->camera_x - vm->offset_x;
       y = sim->school_y*vm->world_to_pixel - (vm->window_height/2) + vm->camera_y - vm->offset_y - vm->offset_view_y;
    }

    painter->drawEllipse(QPoint(x, y), 5, 5);

//    cout<<"Sprite "<<this->distribution_center
//        <<" x, y : "<<x<<" | "<<y
//        <<" wcx, wcy : "<<vm->window_center_x<<" | "<<vm->window_center_y
//        <<" offx, offy : "<<vm->offset_x<<" | "<<vm->offset_y
//        <<endl;
}

