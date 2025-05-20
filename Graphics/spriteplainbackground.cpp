/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "spriteplainbackground.h"

SpritePlainBackground::SpritePlainBackground()
{

}


void SpritePlainBackground::setColor(QColor c){
    //QColor(0,0,0,255)
    color = c;
}

void SpritePlainBackground::draw(QPainter *painter, ViewModifier* vm, int frame){
    if (dark_mode){
        QColor color = QColor(30, 30, 30, 255);
        painter->setPen(color);
        painter->setBrush(color);
    }
    else{
        painter->setPen(this->color);
        painter->setBrush(this->color);
    }
    painter->drawRect(QRect(0, 0, vm->window_width, vm->window_height));
}
