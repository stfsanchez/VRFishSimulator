/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef SPRITECENTER_H
#define SPRITECENTER_H

#include "Graphics/sprite.h"

class SpriteCenter : public Sprite
{
public:
    Simulator* sim;

    bool distribution_center = false;

    double x = 0;
    double y = 0;

    QColor color = Qt::red;

    SpriteCenter(Simulator* sim);

    void setColor(QColor color);

    void setDistributionCenter(bool value);

    void draw(QPainter*  painter, ViewModifier* vm, int frame);
};

#endif // SPRITECENTER_H
