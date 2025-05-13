/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef SPRITEGRID_H
#define SPRITEGRID_H

#include "Graphics/sprite.h"

class SpriteGrid : public Sprite
{
public:
    double origin_x;
    double origin_y;
    double square_length;
    double x;
    double y;
    bool dark_mode = false;

    QColor color = Qt::gray;

    SpriteGrid(double square_size=10.0);

    void setColor(QColor c);

    void draw(QPainter *painter, ViewModifier* vm, int frame);

};

#endif // SPRITEGRID_H
