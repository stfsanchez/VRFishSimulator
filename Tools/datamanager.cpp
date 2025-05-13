/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "datamanager.h"

#include "globalconstants.h"

#include "Agent/controller3d.h"
#include "Agent/controller3dconstants.h"
#include "datamanagerconstants.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <direct.h>
#include <filesystem>

using namespace std;

DataManager::DataManager(Simulator *nsim) {
    this->sim = nsim;

    writedata = writeData;
    writeimg = writePictures;
    writepdf = writePDF;
    writetrajectories = writeTrajectories;

    it_min = (int)(T_MIN/DT + 0.5);
    it_max = (int)((SIM_TIME + T_MIN) / DT + 0.5);
    it_traj   = int(t_traj / DT + 0.5);
    it_dt_traj = (int)(dt_traj / DT + 0.5);
    it_traj = (int)(it_traj / it_dt_traj) * it_dt_traj;
    it_start = it_min;
    interval_traj = int(dt_interval_traj / DT + 0.5);
    itest_traj = 0;
}

void DataManager::createFoldersAndFiles(){
    //Get date string
    time_t curr_time;
    tm * curr_tm;
    char date_string[100];
    char time_string[100];
    char file_string[100];

    time(&curr_time);
    curr_tm = localtime(&curr_time);
    strftime(date_string, 100, "%Y-%m-%d", curr_tm);
    strftime(time_string, 100, "%H-%M", curr_tm);
    strftime(file_string, 100 ,"%Y%m%d_%H%M", curr_tm);

    int n_fish;
    if (NB_FISH_NETWORK > 0){
        n_fish = NB_FISH_NETWORK;
    }
    else{
        n_fish = NB_FISH;
    }

    //Create folders
    stringstream ssfd;
    ssfd << dataFolder << date_string << "/" << n_fish << "Fish/"<<time_string<<"/";
    this->foldername = ssfd.str();

    if (this->writeimg){
        ssfd << "Pictures/";
        this->picturesfoldername = ssfd.str();
    }

    stringstream ssfl;
    ssfl << file_string << "_" << NB_FISH << "Fish";
    this->filename = ssfl.str();

    stringstream dataname;
    dataname << this->foldername << this->filename << ".txt";
    WORLD_FILE = dataname.str();

    if (this->writedata || this->writeimg){
        //_mkdir(this->foldername.c_str());
        filesystem::create_directories(this->foldername.c_str());
    }

    if (this->writedata){
        ofstream datafile(dataname.str().c_str());
        datafile.close();
    }

    if (this->writeimg){
        //_mkdir(this->picturesfoldername.c_str());
        filesystem::create_directories(this->picturesfoldername.c_str());
    }
}

void DataManager::setWriteFlags(bool writedata, bool writeimg){
    this->writedata = writedata;
    this->writeimg = writeimg;
}

void DataManager::writeFishData(){
    if (writedata and sim->simSteps > it_min){
        stringstream dataname;
        dataname << foldername << filename << ".txt";
        ofstream datafile(dataname.str().c_str(), std::ios_base::app);

        datafile.precision(8);

        //datafile<<sim->actualTime<<" ";
        datafile<<(sim->simSteps-it_min)*DT<<" ";
        for (unsigned int i=0; i < sim->fish.size()-1; i++){
            datafile<<sim->fish[i]->toString()<<" ";
        }
        datafile<<sim->fish[sim->fish.size()-1]->toString()<<"\n";

        datafile.close();
    }
}

void DataManager::writePicture(QPixmap pix, int imgfilecount){
    if (writeimg and sim->simSteps>it_min){
        stringstream imgName;
        imgName << picturesfoldername << filename <<"_" << imgfilecount << ".png";
        pix.save(imgName.str().c_str());
    }
}

void DataManager::computePDF(){
    int k;
    double Dphi;

    if (sim->simSteps > it_min and writepdf){
        dangle = M_PI / n_angle;

        for (int i=0; i < sim->fish.size(); i++){
            Fish * fi = sim->fish[i];
            if (fi->ctrl->type == "3d"){
                Controller3D* ci = (Controller3D*)fi->ctrl;

                k = (int)(fi->v / dv);
                dist_v[k] = dist_v[k] + 1.0;

                k = (int)(abs(fi->v - ci->v_av) / dv);
                dist_dv[k] = dist_dv[k] + 1.0;

                if (ci->r_w >= 0.0){
                    k = (int)(ci->r_w / dr_w );
                    dist_r_w[k] = dist_r_w[k] + 1.0;
                }
                else{
                    if (No_Wall != 1){
                        cout<<"ALERT r_w <0"<<endl;
                    }
                }

                k = (int)(ci->r / dr_w);
                dist_r[k] = dist_r[k] + 1.;

                if (symmetrize == 1){
                    k = (int)(n_angle + ci->theta_w / dangle);// - n_angle;
                    dist_theta_w[k] = dist_theta_w[k] + 0.5;
                    k = (int)(n_angle - ci->theta_w / dangle); // - n_angle;
                    dist_theta_w[k] = dist_theta_w[k] + 0.5;
                }
                else{
                    k = (int)(n_angle + ci->theta_w / dangle);// - n_angle;
                    dist_theta_w[k] = dist_theta_w[k] + 1.0;
                }

                k = (int)(n_angle + fi->chi / dangle * 2);
                dist_chi[k] = dist_chi[k] + 1.0;

                if (fi->v_z > v_z_min){
                    k = (int)((fi->v_z - v_z_min) / dv_z);
                    dist_v_z[k] = dist_v_z[k] + 1.0;
                }

                k = int(fi->z / d_z);
                dist_z[k] = dist_z[k] + 1.0;

                if (No_Int != 1){
                    for (int j=0; j<this->sim->fish.size(); j++){
                        Fish * fj = this->sim->fish[j];
                        if (fi->inSimID != fj->inSimID){
                            k = (int)(ci->d_3D[j] / dd);
                            dist_d[k] = dist_d[k] + 1.0;

                            if ((fi->z - fj->z) > d_z_min){
                                k = (int)((fi->z - fj->z - d_z_min) / dd_z);
                                dist_d_z[k] = dist_d_z[k] + 1.0;
                            }

                            if (symmetrize == 1){
                                k = (int)(n_angle + ci->psi_arr[j]/ dangle);// - n_angle;
                                dist_psi[k] = dist_psi[k] + 0.5;
                                k = (int)(n_angle - ci->psi_arr[j] / dangle);// - n_angle;
                                dist_psi[k] = dist_psi[k] + 0.5;
                            }
                            else{
                                k = (int)(n_angle + ci->psi_arr[j] / dangle);// - n_angle;
                                dist_psi[k] = dist_psi[k] + 1.0;
                            }

                            Dphi = ci->normalizeAngle(fj->phi - fi->phi);
                            k = (int)(n_angle + Dphi / dangle);// - n_angle;
                            dist_Dphi[k] = dist_Dphi[k] + 1.0;
                        }
                    }
                }
            }
        }
    }
}

void DataManager::computeRealFishPDF(){
    int k;
    double Dphi;

    if (sim->simSteps > it_min and writepdf){
        dangle = M_PI / n_angle;

        for (int i=0; i < sim->fish.size(); i++){
            Fish * fi = sim->fish[i];
            if (fi->ctrl->type == "network"){
                Controller3D* ci = (Controller3D*)fi->ctrl;

                k = (int)(fi->v / dv);
                dist_real_v[k] = dist_v[k] + 1.0;

                k = (int)(abs(fi->v - ci->v_av) / dv);
                dist_real_dv[k] = dist_dv[k] + 1.0;

                if (ci->r_w >= 0.0){
                    k = (int)(ci->r_w / dr_w );
                    dist_real_r_w[k] = dist_real_r_w[k] + 1.0;
                }
                else{
                    if (No_Wall != 1){
                        cout<<"ALERT r_w <0"<<endl;
                    }
                }

                k = (int)(ci->r / dr_w);
                dist_real_r[k] = dist_real_r[k] + 1.;

                if (symmetrize == 1){
                    k = (int)(n_angle + ci->theta_w / dangle);// - n_angle;
                    dist_real_theta_w[k] = dist_real_theta_w[k] + 0.5;
                    k = (int)(n_angle - ci->theta_w / dangle); // - n_angle;
                    dist_real_theta_w[k] = dist_real_theta_w[k] + 0.5;
                }
                else{
                    k = (int)(n_angle + ci->theta_w / dangle);// - n_angle;
                    dist_real_theta_w[k] = dist_real_theta_w[k] + 1.0;
                }

                k = (int)(n_angle + fi->chi / dangle * 2);
                dist_real_chi[k] = dist_real_chi[k] + 1.0;

                if (fi->v_z > v_z_min){
                    k = (int)((fi->v_z - v_z_min) / dv_z);
                    dist_real_v_z[k] = dist_real_v_z[k] + 1.0;
                }

                k = int(fi->z / d_z);
                dist_real_z[k] = dist_real_z[k] + 1.0;

                if (No_Int != 1){
                    for (int j=0; j<this->sim->fish.size(); j++){
                        Fish * fj = this->sim->fish[j];
                        if (fi->inSimID != fj->inSimID){
                            k = (int)(ci->d_3D[j] / dd);
                            dist_real_d[k] = dist_real_d[k] + 1.0;

                            if ((fi->z - fj->z) > d_z_min){
                                k = (int)((fi->z - fj->z - d_z_min) / dd_z);
                                dist_real_d_z[k] = dist_real_d_z[k] + 1.0;
                            }

                            if (symmetrize == 1){
                                k = (int)(n_angle + ci->psi_arr[j]/ dangle);// - n_angle;
                                dist_real_psi[k] = dist_real_psi[k] + 0.5;
                                k = (int)(n_angle - ci->psi_arr[j] / dangle);// - n_angle;
                                dist_real_psi[k] = dist_real_psi[k] + 0.5;
                            }
                            else{
                                k = (int)(n_angle + ci->psi_arr[j] / dangle);// - n_angle;
                                dist_real_psi[k] = dist_real_psi[k] + 1.0;
                            }

                            Dphi = ci->normalizeAngle(fj->phi - fi->phi);
                            k = (int)(n_angle + Dphi / dangle);// - n_angle;
                            dist_real_Dphi[k] = dist_real_Dphi[k] + 1.0;
                        }
                    }
                }
            }
        }
    }
}

void DataManager::computeTrajectories(){
    int it = sim->simSteps;
    int tsize = trajectories.size();
    if (it >= it_min && tsize <= n_traj && writetrajectories){
        if ((it - it_min) % interval_traj == 0 && itest_traj==0){
            //New trajectory
            it_start = it;
            tsize += 1;
            trajectories.insert({tsize, std::vector<string>()});
            itest_traj = 1;
        }
        //Continue current trajectory
        if ((it - it_start) % it_dt_traj == 0 && itest_traj==1){
            double t = (it - it_start)*DT;
            std::stringstream ss;
            ss<<t<<" ";
            for (int i=0; i<sim->fish.size()-1; i++){
                ss<<sim->fish[i]->trajectoryData()<<" ";
            }
            ss<<sim->fish[sim->fish.size()-1]->trajectoryData()<<"\n";
            trajectories[tsize].push_back(ss.str());
        }
        if (it > it_min and (it - it_min) % interval_traj == it_traj and itest_traj == 1){
            itest_traj = 0;
        }
    }
}

void DataManager::savePDF(){
    if (writepdf){
        cout<<"Saving PDF ...\n"<<endl;
        double x_val, y_val;

        double dist_norm  =(double)(sim->simSteps - it_min) * NB_FISH_3D_MODEL;
        double dist_norm2 = (double)(sim->simSteps - it_min) * NB_FISH_3D_MODEL * (sim->fish.size() - 1);

        stringstream pdfname;
        pdfname << this->foldername << this->filename <<"_pdf.dat";

        ofstream pdffile(pdfname.str().c_str());
        pdffile.precision(8);

        pdffile<<"#   v              ρ(v)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_v.size(); k++){
            if (dist_v[k]>0.5){
                x_val = k * dv + 0.5 * dv;
                y_val = dist_v[k] / (dist_norm * dv);
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   dv             ρ(dv)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_dv.size(); k++){
            if (dist_dv[k]>0.5){
                x_val = k * dv + 0.5 * dv;
                y_val = dist_dv[k] / (dist_norm * dv);
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   v_z             ρ(v_z)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_v_z.size(); k++){
            if (dist_v_z[k]>0.5){
                x_val = v_z_min + k * dv_z + 0.5 * dv_z;
                y_val = dist_v_z[k] / dist_norm / dv_z;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   r_w             ρ(r_w)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_r_w.size(); k++){
            if (dist_r_w[k]>0.5){
                x_val = k * dr_w + 0.5 * dr_w;
                y_val = dist_r_w[k] / dist_norm / dr_w;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   r              ρ(r)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_r.size(); k++){
            if (dist_r[k]>0.5){
                x_val = k * dr_w + 0.5 * dr_w;
                y_val = dist_r[k] / dist_norm / dr_w;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   theta_w        ρ(theta_w)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_theta_w.size(); k++){
            if (dist_theta_w[k]>0.5){
                x_val = k * dangle + 0.5 * dangle - M_PI;
                y_val = dist_theta_w[k] / dist_norm / dangle;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   chi            ρ(chi)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_chi.size(); k++){
            if (dist_chi[k]>0.5){
                x_val = (k * dangle + 0.5 * dangle - M_PI) / 2;
                y_val = dist_chi[k] / dist_norm / dangle * 2;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   z              ρ(z)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_z.size(); k++){
            if (dist_z[k]>0.5){
                x_val = k * d_z + 0.5 * d_z;
                y_val = dist_z[k] / dist_norm / d_z;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   d              ρ(d)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_d.size(); k++){
            if (dist_d[k]>0.5){
                x_val = k * dd + 0.5 * dd;
                y_val = dist_d[k] / dist_norm2 / dd;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   d_z            ρ(d_z)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_d_z.size(); k++){
            if (dist_d_z[k]>0.5){
                x_val = d_z_min + k * dd_z + 0.5 * dd_z;
                y_val = dist_d_z[k] / dist_norm2 / dd_z;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   psi            ρ(psi)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_psi.size(); k++){
            if (dist_psi[k]>0.5){
                x_val = k * dangle + 0.5 * dangle - M_PI;
                y_val = dist_psi[k] / dist_norm2 / dangle;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile<<"#   Dphi           ρ(Dphi)\n";
        pdffile<<"&\n";
        for (int k=0; k<dist_Dphi.size(); k++){
            if (dist_Dphi[k]>0.5){
                x_val = k * dangle + 0.5 * dangle - M_PI;
                y_val = dist_Dphi[k] / dist_norm2 / dangle;
                pdffile<<x_val<<" "<<y_val<<"\n";
            }
        }
        pdffile<<"\n";

        pdffile.close();

        cout<<"PDF saved in "<<pdfname.str()<<endl;
    }
}

void DataManager::saveTrajectories(){
    if (writetrajectories){
        cout<<"Saving trajectories ...\n"<<endl;

        for (int i=1; i<=trajectories.size(); i++){
            stringstream trajname;
            trajname << this->foldername << this->filename << "_traj_"<<i<<".dat";

            ofstream trajfile(trajname.str().c_str());
            trajfile.precision(8);

            vector<string> traj = trajectories[i];
            for (int j=0; j<traj.size();j++){
                trajfile<<traj[j];
            }
            trajfile.close();
        }
        cout<<"Trajectories saved ! \n"<<endl;
    }
}




