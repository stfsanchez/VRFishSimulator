#include "spritebackground.h"

#include <iostream>
#include <cmath>
#include <direct.h>

SpriteBackground::SpriteBackground()
{
    char * dirpath = NULL;
    dirpath = _getcwd(NULL, 0); // or _getcwd
    string fullFilepath = string(dirpath);
    fullFilepath = fullFilepath + "/Data/Grid_background_XL.png";
    QString qs(fullFilepath.c_str());
    this->image = QPixmap(qs);

    x = -1000;
    y = -1000;
}

void SpriteBackground::draw(QPainter *painter, ViewModifier* vm, int frame){
    double dx = vm->dx;
    double dy = vm->dy;

    double epsilon_x = vm->window_width/10;
    double epsilon_y = vm->window_height/10;

    if (frame == 0){
        //if (std::abs(dx) < 10 && std::abs(dy) < 10){
            x = x - dx;
            y = y - dy;
        //}

        if (y < -(vm->window_height-epsilon_y)*2){
            y += (vm->window_height-epsilon_y)*2;
        }
        else if ( y > 50){
            y -= (vm->window_height-epsilon_y)*2;
        }

        if (x < -(vm->window_width-epsilon_x)*2){
            x += (vm->window_width-epsilon_x)*2;
        }
        else if ( x > 50){
            x -= (vm->window_width-epsilon_x)*2;
        }
    }

    //cout<<"BG @"<<vm->sim->simSteps<<" dx, dy : "<<vm->dx<<" | "<<vm->dy<<" x, y :"<<x<<" "<<y<<endl;

    painter->drawPixmap(x, y, 2000,2000, image);
}
