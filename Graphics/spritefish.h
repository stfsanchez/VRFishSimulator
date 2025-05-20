/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef SPRITEFISH_H
#define SPRITEFISH_H

#include <QtWidgets/QGraphicsPolygonItem>

#include "sprite.h"
#include "../Agent/fish.h"

class SpriteFish: public Sprite
{
public:
    Fish * fish;

    QColor color = Qt::gray;
    QColor direction_color = Qt::darkRed;
    QColor trajectory_color = Qt::gray;

    bool show_fish = true;
    bool show_trajectory = false;
    int trajectory_length = 10;
    int trajectory_width = 1;
    vector<QPointF> trajectory;

    bool show_neighbors = false;

    void setFish(Fish * newFish);
    void setColor(QColor c);
    void setDirectionColor(QColor c);
    void setTrajectoryColor(QColor c);
    void setShowFish(bool value);
    void setShowTrajectory(bool value);
    void setTrajectoryLength(int length);
    void setTrajectoryWidth(int width);
    void setShowNeighbors(bool value);

    void draw(QPainter *painter, ViewModifier* vm, int frame);
};

#endif // SPRITEFISH_H
