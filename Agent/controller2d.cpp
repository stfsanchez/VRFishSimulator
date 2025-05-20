/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-SnippetCopyrightText: 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "controller.h"
#include "../Core/simulator.h"
#include "controller2d.h"
#include "controller2dconstants.h"
#include "Tools/rngsingleton.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

using namespace std;

Controller2D::Controller2D()
{
    type = "2d";
    this->initDone = false;
    this->onePassAction = false;
}

/**
 * @brief Calcul de la variation d'angle aléatoire
 *
 * @param g_number g sera tiré utilisant une fonction trouvÃ©e par function.h
 * @param gamma_random valeur fixe
 * @return double
 */
double Controller2D::d_phi_random(double g_number, double gamma_random)
{
    double d_phi_r = gamma_random * g_number;
    return d_phi_r;
}

double Controller2D::angle_vue(Fish* neighbor)
{
    double psi;
    psi = this->fish->phi - atan2(neighbor->y - this->fish->y, neighbor->x - this->fish->x);
    return psi;
}

double Controller2D::distance_entre_poissons(Fish* neighbor)
{
    double d;
    d = sqrt(pow((this->fish->x - neighbor->x),2) + pow((this->fish->y - neighbor->y),2) + pow((this->fish->z - neighbor->z),2));
    return d;
}

double Controller2D::interaction_wall()
{
    double dphi_wall = 0.;
    double r_wall = 0.;
    double fw = 0.;
    double ow = 0.;
    double dphiwsym = 0.;
    double dphiwasym = 0.;

    double theta = atan2(this->fish->y, this->fish->x);
    double theta_w = (this->fish->phi - theta);


    switch (this->model_mode){
        case 1:
            r_wall = sqrt(arena_radius*arena_radius) - sqrt(this->fish->x*this->fish->x +  this->fish->y*this->fish->y); //- this->fish->z*this->fish->z
            dphi_wall = gamma_wall * (exp(-pow((r_wall / l_w), 2)) * sin(theta_w) * (1 + (epsilon_2 * cos(2 * theta_w))));
            break;
        default :
            r_wall = arena_radius - std::sqrt(this->fish->x * this->fish->x + this->fish->y * this->fish->y);
            this->fw = std::exp(-std::pow(r_wall / dw, 2));
            ow = std::sin(theta_w) * (1. + 0.7 * std::cos(2. * theta_w));
            //dphiwsym = gamma_sym * std::sin(2. * theta_w) * std::exp(-r_wall / dw) * r_wall / dw;
            //dphiwasym = gamma_asym * std::cos(theta_w) * std::exp(-r_wall / dw) * r_wall / dw;
            //dphi_wall += dphiwsym + dphiwasym;
            dphi_wall = gamma_wall * this->fw * ow;
            break;
    }
    return dphi_wall;
}

/**
 *
 *
 */
double Controller2D::interaction_fish(Fish* neighbor)
{
    double O_att = 0.;
    double E_att = 0.;
    double F_ali = 0.;
    double O_ali;
    double E_ali;

    double d = this->distance_entre_poissons(neighbor);
    double psi = this->angle_vue(neighbor);
    double dphi = neighbor->phi - this->fish->phi;

    double F_att = ((d - A) / A) / (1 + pow(d / L_att, 2));

    switch (this->model_mode){
        case 1:
            O_att = -sin(psi) * (1 - 0.33 * cos(psi));
            E_att = 1 - 0.48 * cos(dphi) - 0.31 * cos(2 * dphi);
            F_ali = (d / L_ali + C) * exp(-(d / L_ali) * (d / L_ali));
            O_ali = sin(dphi) * (1 + 0.3 * cos(2 * dphi));
            E_ali = 1. + 0.6 * cos(psi) - 0.32 * cos(2. * psi);
            break;
        default:
            O_att = -std::sin(psi) * (1. + 0.5 * std::cos(psi));
            E_att = 1.;
            F_ali = std::exp(-std::pow(d / L_ali, 2));
            O_ali = sin(dphi);
            E_ali = (1. + 0.5 * std::cos(psi));
            break;
    }

    double dphi_attraction = gamma_attraction * F_att * O_att * E_att;

    double dphi_alignement = gamma_alignement * O_ali * E_ali * F_ali;

    double dphi_fish = dphi_attraction + dphi_alignement;

    return dphi_fish;
}

void Controller2D::initFish(){
    double theta_init;
    double r_init;
    double r;

    this->num_steps = 0;
    this->num_kick = 0;

    this->kicktime = this->sim->actualTime;
    this->kickspeed = vmean;
    this->newspeed = vmean;

    double predx, predy, predz;

    this->newchi = 0;

    double dtau = taumean - taumin;

    do
    {
        this->newtau = taumin - dtau * 0.5 * log(rndm()*rndm());
        r_init = arena_radius * std::pow(rndm(), 1./4.);
        theta_init = M_PI * (this->fish->inSimID + 1) / 16. * rndm();

        this->kickx = r_init * cos(theta_init);
        this->kicky = r_init * sin(theta_init);
        this->kickz = zmean;

        this->newphi = (theta_init + M_PI_2);

        this->dl_next = this->newspeed * tau0 * (1 - exp(-this->newtau / tau0));
        predx = kickx + this->dl_next * cos(this->newphi) * cos(this->newchi);
        predy = kicky + this->dl_next * sin(this->newphi) * cos(this->newchi);
        predz = kickz + this->dl_next * sin(this->newchi);

        r = sqrt(predx * predx + predy * predy); // + predz * predz);

    } while (r > arena_radius); // || this->newz < z0);

    this->newx = this->kickx;
    this->newy = this->kicky;
    this->newz = this->kickz;

    this->kicking = true;

    this->fish->active = true;

    this->initDone = true;
}

void Controller2D::glide(){
    //int it = this->sim->simSteps;
    //double dt_now = it*dt - this->dT;
    double dt = this->sim->actualTime - this->kicktime;
    double expt = std::exp(-dt/tau0);
    double dl = this->kickspeed * tau0 * (1 - expt);

    this->newx = this->kickx + dl*cos(this->fish->phi)*cos(this->fish->chi);
    this->newy = this->kicky + dl*sin(this->fish->phi)*cos(this->fish->chi);
    this->newz = this->kickz + dl*sin(this->fish->chi);
    this->newchi = this->fish->chi;
    this->newphi = this->fish->phi;
    this->newspeed = this->kickspeed*expt;
}

void Controller2D::computeNeighbors(){
    this->neighbors.clear();
    for (unsigned int i=0; i<this->sim->fish.size(); i++){
        Fish *f = this->sim->fish[i];
        if (f->inSimID != this->fish->inSimID){
            this->neighbors.push_back(i);
        }
    }
}

void Controller2D::computeInteractions(){
    this->computeNeighbors();

    // Wall interaction
    this->dphi_wall = interaction_wall();

    // Neigbors interactions
    this->dphi_fish = 0.0;
    this->dchi_fish = 0.0; //z

    for (unsigned int i=0; i<this->neighbors.size(); i++) {
        Fish*  n = this->sim->fish[i];
        this->dphi_fish += interaction_fish(n);

        // z
        this->dchi_fish -= gamma_z * tanh((this->fish->z - n->z)/A);
        this->dchi_fish -= gamma_z_par*sin(this->fish->chi - n->chi);
    }

    // z
    this->dchi_par = - gamma_par_chi * sin(2 * this->fish->chi);
    this->dchi_per = - gamma_per_chi * tanh((this->fish->z - zmean) / A);
}

bool Controller2D::checkKick(){
    double bcDuration = this->tau;
    double timeSpent = this->sim->actualTime - this->kicktime;
    bool test = timeSpent > bcDuration;
    return test;//|| this->fish->speed<vmin);
}

void Controller2D::kick(){
    //double v0_old = this->fish->speed;
    //double v_old = this->fish->speed * std::exp(-this->tau / tau_0);

    double predx, predy, predz;

    //Kick !
    this->num_kick++;
    this->kicktime = this->sim->actualTime;
    this->kickx = this->fish->x;
    this->kicky = this->fish->y;
    this->kickz = this->fish->z;

    this->computeInteractions();

    double newphi0 = (this->fish->phi + this->dphi_fish + this->dphi_wall);

    double theta = atan2(this->fish->y, this->fish->x);
    double theta_w = (this->fish->phi - theta);

    int iter = 0;
    double predr;
    do{
        do{
            this->kickspeed = vmin + vmean * (-log(rndm()*rndm()*rndm())) / 3.;
        } while (this->kickspeed > vcut);

        this->newspeed = this->kickspeed;

        double dtau = taumean - taumin;
        this->newtau = taumin - dtau * 0.5 * log(rndm()*rndm());

        //Cognitive noise
        double gauss = std::sqrt(-2. * log(rndm())) * cos(2 * M_PI * rndm());
        this->newphi = (newphi0 + gamma_rand * gauss * (1. - alpha_w * this->fw));
        this->newchi = 0.;
        this->dl_next = this->kickspeed * tau0 * (1. - std::exp(-this->newtau / tau0)) + dc;
        predx = this->kickx + this->dl_next * cos(this->newphi) * cos(this->newchi);
        predy = this->kicky + this->dl_next * sin(this->newphi) * cos(this->newchi);

        //this->newz = this->fish->z + this->dl_next * sin(this->newchi);
        //predz = this->sim->fish[this->neighbors[0]]->z;

        predr = sqrt(predx * predx + predy * predy); // + predz * predz);

        if (++iter>max_kick_computation_tentatives){
            iter = 0;
            double dphiplus = 0.1 * rndm();
            if (theta_w > 0) {
                this->fish->phi = (std::atan2(this->fish->y, this->fish->x) + M_PI_2 + dphiplus);
                this->fish->chi = 0.;
            }
            else {
                this->fish->phi = (std::atan2(this->fish->y, this->fish->x) - M_PI_2 - dphiplus);
                this->fish->chi = 0.;
            }

            this->computeInteractions();
        }
    } while (predr > arena_radius);
}

void Controller2D::act(double dt){
    if (!this->initDone){
        this->initFish();
    }
    else{
        this->num_steps ++;
        this->glide();
        this->kicking = this->checkKick();
        if (this->kicking){
            this->kick();
        }
    }
}

/* V1
void Controller2D::act(double dt){
    if (!this->initDone){
        this->initFish();
        this->updateFish();
    }
    else{
        this->dT = dt;

        if (this->phase == 1){ // COAST
            this->num_steps ++;
            this->glide();
            this->updateFish();
            this->phase = 0;
        }
        else{ // BURST (eventually)
            this->kicking = this->checkKick();
            if (this->kicking){
                this->num_kick++;
                this->kick();
                this->updateFish();
                this->kicking = false;
            }
            this->phase = 1;
        }
    }
}
*/

void Controller2D::update(){
    this->fish->x = this->newx;
    this->fish->y = this->newy;
    this->fish->z = this->newz;
    this->fish->chi = angle_to_pipi(this->newchi);
    this->fish->phi = this->newphi;
    this->fish->v = this->newspeed;

    if (this->kicking){
        this->tau = this->newtau;
        this->kicking = false;
    }
}

void Controller2D::reset(){

}

double Controller2D::rndm(){
    RNGSingleton& rng = RNGSingleton::getInstance();
    return rng.getRandomNumber();
}

double Controller2D::angle_to_pipi(double difference) const
{
    do {
        if (difference < -M_PI)
            difference += 2. * M_PI;
        if (difference > M_PI)
            difference -= 2. * M_PI;
    } while (std::abs(difference) > M_PI);
    return difference;
}
