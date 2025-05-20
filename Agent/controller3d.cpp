/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "controller3d.h"
#include "controller3dconstants.h"
#include "Core\simulator.h"
#include "Tools\rngsingleton.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include <stdio.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

Controller3D::Controller3D() {
    type = "3d";
    this->initDone = false;
    this->onePassAction = false;

    this->noise_para = 0.;
    this->noise_perp = 0.;
    this->noise_z = 0.;
}

void Controller3D::initFish(){
    double rr = 13. + 7.*ran(); //New init
    double dtheta = M_PI/6 * sqrt(this->sim->fish.size()) * ran();

    this->fish->x = rr * cos(dtheta);
    this->fish->y = rr * sin(dtheta);
    this->fish->z = 4.5;
    this->r_w = generate_r_w(this->fish->x, this->fish->y, this->fish->z);
    if (this->r_w < 0.){
        printf("Error in the initial conditions: r_w < 0\n");
        exit(1);
    }
    this->fish->v = C_v[0] - 1.5;
    this->fish->v_z = 0.;
    this->r = sqrt(this->fish->x*this->fish->x + this->fish->y*this->fish->y);
    this->theta_w =  M_PI / 2.;
    this->fish->phi = normalizeAngle(this->theta_w + dtheta);
    this->fish->v_x = this->fish->v * cos(this->fish->phi);
    this->fish->v_y = this->fish->v * sin(this->fish->phi);

    initNoise(this->sim->dt);

    if (K < 1 || K > sim->fish.size()-1){
        K = sim->fish.size() - 1;
    }

    renorm_K = pow(K, renorm_K_pow);

    this->initDone = true;
}

void Controller3D::initNoise(double dt){
    Noise1      = exp(-dt / tau_XY);
    Noise2_para = sigma_para * sqrt(1.0 - exp(-2.0 * dt / tau_XY));
    Noise2_perp = sigma_perp * sqrt(1.0 - exp(-2.0 * dt / tau_XY));
    Noise1_z    = exp(-dt / tau_Z);
    Noise2_z    = sigma_Z * sqrt(1.0 - exp(-2.0 * dt / tau_Z));
}

void Controller3D::computeFishSchoolData(){
    double dx, dy, dz;
    this->d_2D.clear();
    this->d_3D.clear();
    this->psi_arr.clear();
    this->v_av = 0.;
    for (int i=0; i<this->sim->fish.size(); i++){
        Fish* f = sim->fish[i];
        if (f->inSimID != this->fish->inSimID) {
            v_av += f->v;
            //cout<<f->v<<endl;
            dx = f->x - this->fish->x;
            dy = f->y - this->fish->y;
            dz = f->z - this->fish->z;
            d_2D.push_back(sqrt(dx*dx + dy*dy));
            d_3D.push_back(sqrt(dx*dx + dy*dy + dz*dz));
            psi_arr.push_back(normalizeAngle(this->fish->phi - atan2(dy, dx)));
        }
        else{
            d_2D.push_back(0.);
            d_3D.push_back(0.);
            psi_arr.push_back(0.);
        }
    }
    this->v_av /= (this->sim->fish.size()-1);
}

void Controller3D::swim(double dt){
    double dt2 = 0.5 * dt;
    double a_x, a_y, a_z, Dphi, dx, dy;
    double x_c, y_c, z_c;

    computeFishSchoolData();

    double friction_v = Coef_Friction * Friction(this->fish->v, C_v); // + Coef_Adapt * Adapt(this->fish->v - this->v_av, C_v); //Todo : check later
    double force_wall  = Coef_f_w * f_w(this->r_w, C_f_w) * g_w(this->theta_w, C_w);
    double force_rot   = Coef_f_rot * f_rot(this->r_w, C_f_rot) * g_rot(this->theta_w, C_rot);

    double rr = r + epsilon;
    double vv = fish->v + epsilon;

    a_x = - (this->fish->v_x / vv) * friction_v - (this->fish->x / rr) * force_wall - sin(this->fish->phi) * force_rot;
    a_y = - (this->fish->v_y / vv) * friction_v - (this->fish->y / rr) * force_wall + cos(this->fish->phi) * force_rot;

    double friction_v_z = Coef_Friction_z * Friction_z(this->fish->v_z, C_v_z);
    double force_z = Coef_f_z * f_z(this->fish->z, C_f_z);
    a_z = - friction_v_z - force_z;

    /* Interactions */
    if (No_Int != 1) {
        int n = this->sim->fish.size();
        std::vector<double> force_int_x(n, 0.0);
        std::vector<double> force_int_y(n, 0.0);
        std::vector<double> force_int_z(n, 0.0);
        std::vector<double> force_int(n, 0.0);

        for (int i = 0; i < this->sim->fish.size(); i++) {
            Fish* f = this->sim->fish[i];
            if (f->inSimID != this->fish->inSimID) {
                Dphi = f->phi - this->fish->phi;
                dx = (f->x - this->fish->x) / (d_2D[i]+epsilon);
                dy = (f->y - this->fish->y) / (d_2D[i]+epsilon);

                double force_att = Coef_f_att * f_att(d_3D[i], C_f_att_d) * E_att(psi_arr[i], C_att) * Ep_att(Dphi, D_att);
                double force_ali = Coef_f_ali * f_ali(d_3D[i], C_f_ali_d) * E_ali(psi_arr[i], C_ali) * Ep_ali(Dphi, D_ali) * sin(Dphi);
                double force_att_z = Coef_f_att_z * f_att_z(f->z - this->fish->z, C_f_att_z) * f_att_z_d(d_3D[i], C_f_att_z_d) * E_att_z(psi_arr[i], C_att_z) * Ep_att_z(Dphi, D_att_z);

                force_int_x[i] = force_att * dx - force_ali * sin(this->fish->phi);
                force_int_y[i] = force_att * dy + force_ali * cos(this->fish->phi);
                force_int_z[i] = force_att_z;

                force_int[i] = force_int_x[i] * force_int_x[i] + force_int_y[i] * force_int_y[i] + force_int_z[i] * force_int_z[i];
            }
        }

        if (strategy != 1){ //K-MIN
            std::vector<int> indices(n);

            // Get indices of the K largest values of force_int (order not guaranteed)
            // to obtain the K most influential neighbors
            std::iota(indices.begin(), indices.end(), 0);
            std::partial_sort(indices.begin(), indices.begin() + K, indices.end(),
                              [&force_int](int a, int b) { return force_int[a] > force_int[b]; });

            for (int j = 0; j < K; ++j) {
                int idx = indices[j];
                a_x += force_int_x[idx] / renorm_K;
                a_y += force_int_y[idx] / renorm_K;
                a_z += force_int_z[idx] / renorm_K;
            }
        }
        else{ //K-NN
            std::vector<int> indices(n);

            // Get indices of the K smallest NON ZERO values of d_3D (order not guaranteed)
            // to obtain the K nearest neighbors
            for (int i=0; i < d_3D.size(); i++){
                if (d_3D[i]>0){
                    indices.push_back(i);
                }
            }
            std::vector<double> tmp_d_3D(d_3D.begin(), d_3D.end());
            std::partial_sort(indices.begin(), indices.begin() + K, indices.end(),
                              [&tmp_d_3D](int a, int b) { return tmp_d_3D[a] < tmp_d_3D[b]; });

            for (int j = 0; j < K; ++j) {
                int idx = indices[j];
                a_x += force_int_x[idx] / renorm_K;
                a_y += force_int_y[idx] / renorm_K;
                a_z += force_int_z[idx] / renorm_K;
            }
        }
    }

    /* Update correlated Gaussian noises */
    //double Pi2 = 2*M_PI;
    //double g_para = sqrt(-2.0 * log(ran())) * cos(Pi2 * ran());
    //double g_perp = sqrt(-2.0 * log(ran())) * cos(Pi2 * ran());
    //double g_z    = sqrt(-2.0 * log(ran())) * cos(Pi2 * ran());
    double g_para = rand();
    double g_perp = rand();
    double g_z    = rand();

    noise_para = noise_para * Noise1 + Noise2_para * g_para;
    noise_perp = noise_perp * Noise1 + Noise2_perp * g_perp;
    noise_z    = noise_z * Noise1_z + Noise2_z * g_z;

    a_x += noise_para * cos(this->fish->phi) - noise_perp * sin(this->fish->phi);
    a_y += noise_para * sin(this->fish->phi) + noise_perp * cos(this->fish->phi);
    a_z += noise_z;

    //New fish values
    v_x_new = fish->v_x + a_x * dt;
    v_y_new = fish->v_y + a_y * dt;
    v_z_new = fish->v_z + a_z * dt;
    x_new = fish->x + (fish->v_x + v_x_new) * dt2;
    y_new = fish->y + (fish->v_y + v_y_new) * dt2;
    z_new = fish->z + (fish->v_z + v_z_new) * dt2;

    v_new =  sqrt(v_x_new*v_x_new + v_y_new*v_y_new + epsilon2);

    phi_new = atan2(v_y_new, v_x_new);
    chi_new = atan(v_z_new / v_new);

    x_c = x_new + d_c * cos(phi_new) * cos(chi_new);
    y_c = y_new + d_c * sin(phi_new) * cos(chi_new);
    z_c = z_new + d_c * sin(chi_new);

    if (No_Wall_z != 1) {
        if (z_c <= 0.0 || z_c >= h_water) {
            n_reject_z++;
            z_new = fish->z;
            v_z_new = -fish->v_z;
        }
    }
    if (No_Wall != 1) {
        double r_c = generate_r_w(x_c, y_c, z_c);
        if (r_c <= 0.0) {
            n_reject_xy++;
            x_new = fish->x;
            y_new = fish->y;
            z_new = fish->z;

            double theta = atan2(y_new, x_new);
            if (theta_w > 0.0)
            {
                //phi_new = theta + M_PI / 2.0 * (1 + dBounce * sqrt(-2.0 * log(ran())) * cos(Pi2 * ran()));
                phi_new = theta + M_PI / 2.0 * (1 + dBounce * ran());
                noise_perp = fabs(noise_perp);
            }
            else
            {
                //phi_new = theta - M_PI / 2.0 * (1 + dBounce * sqrt(-2.0 * log(ran())) * cos(Pi2 * ran()));
                phi_new = theta - M_PI / 2.0 * (1 + dBounce * ran());
                noise_perp = -fabs(noise_perp);
            }
            v_x_new = fish->v * cos(phi_new);
            v_y_new = fish->v * sin(phi_new);
            //v_z_new = 0.0;
            v_new =  sqrt(v_x_new*v_x_new + v_y_new*v_y_new + epsilon2);
        }
    }
}

void Controller3D::update(){
    /* Update positions and speeds */
    fish->x = x_new;
    fish->y = y_new;
    fish->z = z_new;

    fish->v_x = v_x_new;
    fish->v_y = v_y_new;
    fish->v_z = v_z_new;

    //if (v_new>v_cutoff){
    //    v_new = C_v[0];
    //}

    fish->v = v_new;

    //fish->phi = atan2(v_y_new, v_x_new);
    //fish->chi = atan(v_z_new / v_new);
    fish->phi = phi_new;
    fish->chi = chi_new;
    r = sqrt(x_new*x_new + y_new*y_new);
    r_w = generate_r_w(x_new, y_new, z_new);
    theta_w = normalizeAngle(fish->phi - atan2(y_new, x_new));

    //cout<<C_v[0]<<"      "<<v_new<<"     "<<sim->fish[0]->x<<" "<<sim->fish[0]->y<<" "<<sim->fish[0]->z<<"       "<<sim->fish[0]->phi<<" "<<sim->fish[0]->chi<<endl;
}

void Controller3D::act(double dt){
    if (!this->initDone){
        this->initFish();
        //this->initNoise(dt);
    }
    else{
        this->swim(dt);
    }
}

void Controller3D::reset(){
    initFish();
}

/* --------------------------- */
/* Function definitions      */
/* --------------------------- */


/* Friction: computes a polynomial friction function */
double Controller3D::Friction(double v, vector<double> C_v)
{
    //std::cout<< C_v[0] << std::endl;
    double vv = v - C_v[0];
    //cout<<C_v[0]<<" "<<v<<" -> "<<vv<<endl;
    return C_v[1] + C_v[2] * vv + C_v[3] * vv * vv + C_v[4] * vv * vv * vv;
}

double Controller3D::Adapt(double v, vector<double> C_v)
{
    return C_v[5]*v + C_v[6]*v*v + C_v[7]*v*v*v;
}


/* f_w: wall–force function */
double Controller3D::f_w(double r_w, vector<double> C_f_w)
{
    //return C_f_w[0] * exp(-r_w / C_f_w[1]) - C_f_w[2];
    return C_f_w[0] * exp(-r_w / C_f_w[1] - (r_w / C_f_w[2])*(r_w / C_f_w[2])) - C_f_w[3];
}


/* f_rot: rotational–force function */
double Controller3D::f_rot(double r_w, vector<double> C_f_rot)
{
    //return C_f_rot[0] * exp(-r_w / C_f_rot[1]) - C_f_rot[2];
    return C_f_rot[0] * exp(-r_w / C_f_rot[1] - (r_w / C_f_rot[2])*(r_w / C_f_rot[2])) - C_f_rot[3];
}


/* g_w: modulation function */
double Controller3D::g_w(double theta_w, vector<double> C_w)
{
    double f = 1.0;
    int mode;
    for (mode = 0; mode < N_MODES_MAX; mode++) {
        f += C_w[mode] * cos((mode + 1) * theta_w);
    }
    return f;
}


/* g_rot: rotational modulation function */
double Controller3D::g_rot(double theta_w, vector<double> C_rot)
{
    double f = 1.0;
    int mode;
    for (mode = 0; mode < N_MODES_MAX; mode++) {
        f += C_rot[mode] * cos((mode + 1) * theta_w);
    }
    return f;
}


/* f_att: attraction function */
double Controller3D::f_att(double d, vector<double> C_f_att_d)
{
    //return (d - C_f_att_d[2]) / C_f_att_d[0] * exp(-d / C_f_att_d[1]);
    double f = (d - C_f_att_d[2]) / C_f_att_d[0] / pow((1 + pow(d / C_f_att_d[1],2)),C_f_att_d[3]);
    if (d < d_c_att){
        double f_att_min = -10.0;
        f = f - f_c * (pow((d_c_att / (d+0.05)),2) - 1);
        if (f < f_att_min){
            f = f_att_min;
        }
    }
    return f;
}


/* f_ali: alignment function */
double Controller3D::f_ali(double d, vector<double> C_f_ali_d)
{
    //return (d - C_f_ali_d[2]) / C_f_ali_d[0] * exp(-d / C_f_ali_d[1]);
    return (d - C_f_ali_d[2]) / C_f_ali_d[0] / pow(1 + pow((d / C_f_ali_d[1]),2),C_f_ali_d[3]);
}


/* E_att: angular dependence for attraction */
double Controller3D::E_att(double psi, vector<double> C_att)
{
    double f = 1.0;
    int mode;
    for (mode = 0; mode < N_MODES_MAX; mode++) {
        f += C_att[mode] * cos((mode + 1) * psi);
    }
    return f;
}


/* E_ali: angular dependence for alignment */
double Controller3D::E_ali(double psi, vector<double> C_ali)
{
    double f = 1.0;
    int mode;
    for (mode = 0; mode < N_MODES_MAX; mode++) {
        f += C_ali[mode] * cos((mode + 1) * psi);
    }
    return f;
}


/* Ep_att: phase dependence for attraction */
double Controller3D::Ep_att(double Dphi, vector<double> D_att)
{
    double f = 1.0;
    int mode;
    for (mode = 0; mode < N_MODES_MAX; mode++) {
        f += D_att[mode] * cos((mode + 1) * Dphi);
    }
    return f;
}


/* Ep_ali: phase dependence for alignment */
double Controller3D::Ep_ali(double Dphi, vector<double> D_ali)
{
    double f = 1.0;
    int mode;
    for (mode = 0; mode < N_MODES_MAX; mode++) {
        f += D_ali[mode] * cos((mode + 1) * Dphi);
    }
    return f;
}


/* Friction_z: vertical friction */
double Controller3D::Friction_z(double v_z, vector<double> C_v_z)
{
    return C_v_z[0] * v_z + C_v_z[1] * v_z*v_z + C_v_z[2] * v_z*v_z*v_z;
}


/* f_z: vertical force */
double Controller3D::f_z(double z, vector<double> C_f_z)
{
    double zz = z - C_f_z[0];
    return C_f_z[1] + C_f_z[2] * zz + C_f_z[3] * zz*zz +
           C_f_z[4] * zz*zz*zz + C_f_z[5] * zz*zz*zz*zz;
}


/* f_att_z: vertical attraction function */
double Controller3D::f_att_z(double d_z, vector<double> C_f_att_z)
{
    return C_f_att_z[0] * d_z + C_f_att_z[1] * d_z*d_z +
           C_f_att_z[2] * d_z*d_z*d_z + C_f_att_z[3] * d_z*d_z*d_z*d_z;
}


/* f_att_z_d: vertical attraction distance dependence */
double Controller3D::f_att_z_d(double d, vector<double> C_f_att_z_d)
{
    return C_f_att_z_d[0] / (1.0 + pow(d / C_f_att_z_d[1], C_f_att_z_d[2]));
}


/* E_att_z: angular dependence for vertical attraction */
double Controller3D::E_att_z(double psi, vector<double> C_att_z)
{
    double f = 1.0;
    int mode;
    for (mode = 0; mode < N_MODES_MAX; mode++) {
        f += C_att_z[mode] * cos((mode + 1) * psi);
    }
    return f;
}


/* Ep_att_z: phase dependence for vertical attraction */
double Controller3D::Ep_att_z(double Dphi, vector<double> D_att_z)
{
    double f = 1.0;
    int mode;
    for (mode = 0; mode < N_MODES_MAX; mode++) {
        f += D_att_z[mode] * cos((mode + 1) * Dphi);
    }
    return f;
}


/* generate_r_w: compute the wall distance given the bowl geometry */
double Controller3D::generate_r_w(double x_val, double y_val, double z_val)
{
    double r_val = sqrt(x_val * x_val + y_val * y_val);
    double r_w;

    if (geometry == "sphere") {
        double alpha = 1.0;  // Anisotropy factor
        double R0 = 26.19;   // Radius in Z direction
        double R1 = R0 - h_water;  // Distance from water to geometric center
        double R_z = alpha * sqrt(std::abs(R0 * R0 - (R1 + z_val) * (R1 + z_val)));
        r_w = R_z - r_val;
    } else if (geometry == "ellipsoid") {
        double alpha = 1.0;  // Anisotropy factor (overriding previous 1.1296 value)
        double R0 = 26.19;   // Radius in Z direction
        double R1 = R0 - h_water;  // Distance from water to geometric center
        double R_z = alpha * sqrt(std::abs(R0 * R0 - (R1 + z_val) * (R1 + z_val)));
        r_w = R_z - r_val;
    } else { // Default case: Cylinder
        double R0 = 25;  // Radius of the cylinder
        r_w = R0 - r_val;
    }

    return r_w;
}

/* AngleModulo: map an angle into (-Pi, Pi] */
double Controller3D::AngleModulo(double angle)
{
    /* Use fmod; note that Fortran’s modulo is defined differently */
    return fmod(angle + M_PI, 2.*M_PI) - M_PI;
    return angle;
}

double Controller3D::AngleModulo2(double angle)
{
    const double Pi2 = 2.0 * M_PI;
    double new_angle;

    // Use fmod to get the remainder of the division by 2*PI
    new_angle = fmod(angle + M_PI, Pi2);

    // Adjust the angle to be in the range [-Pi, Pi]
    if (new_angle < -M_PI) {
        new_angle += Pi2;
    } else if (new_angle > M_PI) {
        new_angle -= Pi2;
    }

    return new_angle;
}

double Controller3D::normalizeAngle(double angle) {
    angle = std::fmod(angle + M_PI, 2 * M_PI);
    if (angle < 0)
        angle += 2 * M_PI;
    return angle - M_PI;
}

double Controller3D::ran(){
    RNGSingleton& rng = RNGSingleton::getInstance();
    return rng.getRandomNumber();
}

double Controller3D::rand(){
    RNGSingleton& rng = RNGSingleton::getInstance();
    return rng.getGaussianRandomNumber();
}


