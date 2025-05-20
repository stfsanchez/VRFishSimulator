/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "spritetank.h"

SpriteTank::SpriteTank(FishTank * newTank)
{
    tank = newTank;
}

void SpriteTank::setColor(QColor c){
    //QColor(0,0,0,255)
    color = c;
}

void SpriteTank::draw(QPainter * painter, ViewModifier* vm, int frame)
{
    painter->setBrush(this->color);

    painter->drawEllipse(QPoint(tank->cx*vm->world_to_pixel - (vm->window_width/2.0) + vm->camera_x - vm->offset_x ,
                                tank->cy*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y - vm->offset_view_y),
                                (int) (tank->radius*vm->world_to_pixel),
                                (int) (tank->radius*vm->world_to_pixel));

    painter->drawPie((int)((tank->cx - tank->radius -2)*vm->world_to_pixel - (vm->window_width/2.0) + vm->camera_x - vm->offset_x),
                     (int)((tank->cy - tank->radius)*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y + vm->offset_view_y_2),
                     (int)((tank->radius+2)*2*vm->world_to_pixel),
                     (int)(tank->radius*2*vm->world_to_pixel),
                     0*16, -180*16);

    painter->setBrush(this->bgcolor);

    painter->drawRect((int)((tank->cx - tank->radius -2)*vm->world_to_pixel - (vm->window_width/2.0) + vm->camera_x - vm->offset_x),
                      (int)((tank->cy)*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y + vm->offset_view_y_2),
                      (int)((tank->radius+2)*2*vm->world_to_pixel),
                      (int)(4*vm->world_to_pixel));

    painter->drawLine((int)((tank->cx - tank->radius)*vm->world_to_pixel - (vm->window_width/2.0) + vm->camera_x - vm->offset_x),
                      (int)(10*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y + vm->offset_view_y_2),
                      (int)((tank->cx - tank->radius + 55)*vm->world_to_pixel - (vm->window_width/2.0) + vm->camera_x - vm->offset_x),
                      (int)(10*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y + vm->offset_view_y_2));

}
