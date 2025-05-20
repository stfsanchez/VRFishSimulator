/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef SPRITEBACKGROUND_H
#define SPRITEBACKGROUND_H

#include "Graphics/sprite.h"
#include <QtWidgets/QGraphicsPixmapItem>

class SpriteBackground : public Sprite
{
public:
    QPixmap image;

    double x;
    double y;

    SpriteBackground();

    void draw(QPainter *painter, ViewModifier* vm, int frame);
};

#endif // SPRITEBACKGROUND_H
