/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "spritegrid.h"

#include <iostream>

SpriteGrid::SpriteGrid(double square_size)
{
    origin_x = 0;
    origin_y = 0;
    x = 0;
    y = 0;
    square_length = square_size; //cm in real world
    color = QColor(200,200,200,255);
}

void SpriteGrid::setColor(QColor c){
    color = c;
}

void SpriteGrid::draw(QPainter *painter, ViewModifier* vm, int frame){
    painter->setPen(color);

    x = x - vm->dx*vm->world_to_pixel;
    y = y - vm->dy*vm->world_to_pixel;

    double gap = square_length*vm->world_to_pixel;

    if (std::abs(x)>=gap){
        x = x - (x/std::abs(x))*gap;
    }

    if (std::abs(y)>=gap){
        y = y - (y/std::abs(y))*gap;
    }

    int nx = vm->window_width/square_length + 10;

    for (int i=0; i<nx; i++){
        painter->drawLine(x + i*square_length*vm->world_to_pixel,
                          y - vm->window_height,
                          x + i*square_length*vm->world_to_pixel,
                          y + vm->window_height*2);
    }

    int ny = vm->window_height/square_length + 10;
    for (int i=0; i<ny; i++){
        painter->drawLine(x - vm->window_width*2,
                          y + i*square_length*vm->world_to_pixel,
                          x + vm->window_width*2,
                          y + i*square_length*vm->world_to_pixel);
    }

    //cout<<gap<<" x, y : "<<x<<" , "<<y<<endl;

}
