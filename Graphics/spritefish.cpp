/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/


#include "spritefish.h"

#include <QtCore/QPoint>
#include <cmath>
#include <algorithm>
#include <QtGui/QPainter>

void SpriteFish::setFish(Fish *newFish){
    fish = newFish;
    trajectory.clear();
}

void SpriteFish::setColor(QColor c){
    color = c;
}

void SpriteFish::setDirectionColor(QColor c){
    direction_color = c;
}
void SpriteFish::setTrajectoryColor(QColor c){
    trajectory_color = c;
}

void SpriteFish::setShowTrajectory(bool value){
    show_trajectory = value;
}

void SpriteFish::setShowFish(bool value){
    show_fish = value;
}

void SpriteFish::setTrajectoryLength(int length){
    trajectory_length = length;
}

void SpriteFish::setTrajectoryWidth(int width){
    trajectory_width = width;
}

void SpriteFish::setShowNeighbors(bool value){
    show_neighbors = value;
}


void SpriteFish::draw(QPainter * painter, ViewModifier* vm, int frame)
{
    if (fish != NULL){
        /*----- fish body -----*/
        QPointF shape[4];

        shape[0] = QPointF(fish->length/3.0, 0.0); //top
        shape[1] = QPointF(0.0, fish->width/2.0); //right
        shape[2] = QPointF(-2.0*fish->length/3.0, 0.0); //bottom
        shape[3] = QPointF(0.0, - 1.0*fish->width/2.0); //left

        //Rotate fish
        QPointF points[4];
        for (unsigned int i=0; i<4; i++){
           points[i] = QPointF(shape[i].x()*cos(fish->phi) - shape[i].y()*sin(fish->phi),
                               shape[i].x()*sin(fish->phi) + shape[i].y()*cos(fish->phi));
        }

        for (unsigned int i=0; i<4; i++){
            //Scale fish and set it in position
            points[i].setX((fish->x+points[i].x())*vm->world_to_pixel);
            points[i].setY((fish->y+points[i].y())*vm->world_to_pixel);

            //Translate fish in top left origin
            points[i].setX(points[i].x() - (vm->window_width/2.0));
            points[i].setY(points[i].y() - (vm->window_height/2.0));

            if (i==2 && show_trajectory){
                trajectory.push_back(QPointF(points[i].x(), points[i].y()));
            }

            //Translate fish under camera
            points[i].setX(points[i].x() + vm->camera_x - vm->offset_x);
            points[i].setY(points[i].y() + vm->camera_y - vm->offset_y - vm->offset_view_y);
        }

        if (show_fish){
            painter->setPen(QPen(color));
            painter->setBrush(color);
            painter->drawConvexPolygon(points, 4);
        }

        /*----------- fish direction --------*/

        if (show_fish){
            QPointF p1 = QPointF(0.0, 0.0);
            QPointF p2 = QPointF(fish->length/2.0, 0.0);

            //Rotate p2

            double newP2x = p2.x()*cos(fish->phi) - p2.y()*sin(fish->phi);
            double newP2y = p2.x()*sin(fish->phi) + p2.y()*cos(fish->phi);
            p2.setX(newP2x);
            p2.setY(newP2y);

            //Scale and translate

            p1.setX((fish->x+p1.x())*vm->world_to_pixel - (vm->window_width/2.0)  + vm->camera_x - vm->offset_x);
            p1.setY((fish->y+p1.y())*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y - vm->offset_view_y);
            p2.setX((fish->x+p2.x())*vm->world_to_pixel - (vm->window_width/2.0) + vm->camera_x - vm->offset_x);
            p2.setY((fish->y+p2.y())*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y - vm->offset_view_y);


            QLineF line = QLineF(p1,p2);
            painter->setPen(QPen(direction_color));
            painter->drawLine(line);
        }

        /*----------- fish z --------------------------*/

        if (show_fish){
            QPointF center = QPointF(fish->x*vm->world_to_pixel - (vm->window_width/2.0)  + vm->camera_x - vm->offset_x,
                                     (fish->z+10)*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y + vm->offset_view_y_2);
            painter->setPen(QPen(color));
            painter->setBrush(color);
            painter->drawEllipse(center, 2, 2);
        }


        /*----------- fish trajectory trail -----------*/

        if (trajectory.size()>1 && show_trajectory){

            QPen pen = QPen(trajectory_color);
            pen.setWidth(trajectory_width);
            painter->setPen(pen);
            //painter->setBrush(color);

            for (int i=0; i<std::min(trajectory_length, (int)(trajectory.size()-1)); i++ ){

                QLineF lineTmp = QLineF(trajectory[trajectory.size()-i-1].x() + vm->camera_x - vm->offset_x,
                                        trajectory[trajectory.size()-i-1].y() + vm->camera_y - vm->offset_y - vm->offset_view_y,
                                        trajectory[trajectory.size()-i-2].x() + vm->camera_x - vm->offset_x,
                                        trajectory[trajectory.size()-i-2].y() + vm->camera_y - vm->offset_y - vm->offset_view_y);
                painter->drawLine(lineTmp);
            }
        }

        /*------------ Influential neighbors ----------*/

        /*
        if (this->show_neighbors){


            vector<int> n = fish->neighbors;
            for (unsigned int i=0; i<n.size(); i++){
                if (n[i] != -1){
                    QLineF line = QLineF(fish->x*vm->world_to_pixel - (vm->window_width/2.0) + vm->camera_x - vm->offset_x,
                                         fish->y*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y,
                                         vm->sim->fish[n[i]]->x*vm->world_to_pixel - (vm->window_width/2.0) + vm->camera_x - vm->offset_x,
                                         vm->sim->fish[n[i]]->y*vm->world_to_pixel - (vm->window_height/2.0) + vm->camera_y - vm->offset_y);

                    QColor c = QColor(std::min(60*(int)i, 255), std::min(80*(int)i, 255), std::min(200 + 55*(int)i, 255),255);
                    QPen pen = QPen(c);
                    pen.setWidth(1);
                    painter->setPen(pen);
                    painter->drawLine(line);
                }
            }
        }
        */

        /*--- Test sprite position ---*/
        //double sx = fish->x*vm->world_to_pixel + vm->camera_x;
        //double sy = fish->y*vm->world_to_pixel + vm->camera_y;
        //cout<<"Fish "<<fish->inSimID<<" : "<< fish->x << " , " << fish->y
        //    << " | "<< sx << " , " << sy << endl;


    //cout << "Draw fish : "<<fish->inSimID<<" !" << endl;
    }
}



