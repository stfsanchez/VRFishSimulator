/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef SPRITETANK_H
#define SPRITETANK_H

#include "sprite.h"
#include "../Core/fishtank.h"


class SpriteTank: public Sprite
{
public:
    FishTank * tank;

    QColor color = Qt::black;

    QColor bgcolor = Qt::white;

    SpriteTank(FishTank * newTank);

    void setColor(QColor c);

    void draw(QPainter *painter, ViewModifier* vm, int frame);
};

#endif // SPRITETANK_H
