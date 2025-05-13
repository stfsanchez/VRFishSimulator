/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef SPRITESIMPLEBACKGROUND_H
#define SPRITESIMPLEBACKGROUND_H

#include "sprite.h"

class SpritePlainBackground : public Sprite
{
public:
    bool dark_mode = false;

    QColor color = Qt::white;

    SpritePlainBackground();

    void setColor(QColor c);

    void draw(QPainter *painter, ViewModifier* vm, int frame);
};

#endif // SPRITEPLAINBACKGROUND_H
