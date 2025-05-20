/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef SPRITE_H
#define SPRITE_H

#include <QtGui/QPainter>

#include "Graphics/viewmodifier.h"

class Sprite
{
public:
    Sprite();

    virtual void draw(QPainter*  painter, ViewModifier* vm, int frame) = 0;
};

#endif // SPRITE_H
