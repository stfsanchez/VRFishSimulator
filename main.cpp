/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include <QtWidgets/QApplication>
#include "mainwindow.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <direct.h>
#include <time.h>

#include "Tools/parser.h"

#include "Core/simulator.h"
#include "Agent/replaycontroller.h"
#include "Agent/networkcontroller.h"
#include "Agent/controller2d.h"
#include "Agent/controller3d.h"
#include "Agent/circularcontroller.h"
#include "Agent/rosacecontroller.h"
#include "Agent/fixedpositioncontroller.h"
#include "Agent/controller2dconstants.h"
#include "Agent/controller3dconstants.h"
#include "Tools/datamanagerconstants.h"
#include "Tools/rngconstants.h"

#include "globalconstants.h"

using namespace std;

//Simulator
Simulator* sim;

//Managers
DataManager *dm;
NetworkManager *nm;

//Files
std::string foldername;
std::string filename;

void readParametersFile(){
    cerr<<"Reading parameters file ..."<<endl;

    std::ifstream cFile (param_file);
    std::map<std::string, std::string> settings;
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            std::string name = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            //std::cout << name << " " << value << '\n';
            settings[name] = value;
        }

        std::map<std::string, std::string>::const_iterator pos;

        //C_v
        for (int i=0; i < C_v.size(); i++ ){
            C_v[i] = 0.;
            pos = settings.find("C_v["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_v[i] = std::stod(pos->second);
        }

        //C_w
        for (int i=0; i < C_w.size(); i++ ){
            C_w[i] = 0.;
            pos = settings.find("C_w["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_w[i] = std::stod(pos->second);
        }

        //C_rot
        for (int i=0; i < C_rot.size(); i++ ){
            C_rot[i] = 0.;
            pos = settings.find("C_rot["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_rot[i] = std::stod(pos->second);
        }

        //C_att
        for (int i=0; i < C_att.size(); i++ ){
            C_att[i] = 0.;
            pos = settings.find("C_att["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_att[i] = std::stod(pos->second);
        }

        //C_ali
        for (int i=0; i < C_ali.size(); i++ ){
            C_ali[i] = 0.;
            pos = settings.find("C_ali["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_ali[i] = std::stod(pos->second);
        }

        //D_att
        for (int i=0; i < D_att.size(); i++ ){
            D_att[i] = 0.;
            pos = settings.find("D_att["+ std::to_string(i) + "]");
            if (pos != settings.end())
                D_att[i] = std::stod(pos->second);
        }

        //D_ali
        for (int i=0; i < D_ali.size(); i++ ){
            D_ali[i] = 0.;
            pos = settings.find("D_ali["+ std::to_string(i) + "]");
            if (pos != settings.end())
                D_ali[i] = std::stod(pos->second);
        }

        //C_v_z
        for (int i=0; i < C_v_z.size(); i++ ){
            C_v_z[i] = 0.;
            pos = settings.find("C_v_z["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_v_z[i] = std::stod(pos->second);
        }

        //C_f_z
        for (int i=0; i < C_f_z.size(); i++ ){
            C_f_z[i] = 0.;
            pos = settings.find("C_f_z["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_f_z[i] = std::stod(pos->second);
        }

        //C_f_att_z
        for (int i=0; i < C_f_att_z.size(); i++ ){
            C_f_att_z[i] = 0.;
            pos = settings.find("C_f_att_z["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_f_att_z[i] = std::stod(pos->second);
        }

        //C_att_z
        for (int i=0; i < C_att_z.size(); i++ ){
            C_att_z[i] = 0.;
            pos = settings.find("C_att_z["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_att_z[i] = std::stod(pos->second);
        }

        //D_att_z
        for (int i=0; i < D_att_z.size(); i++ ){
            D_att_z[i] = 0.;
            pos = settings.find("D_att_z["+ std::to_string(i) + "]");
            if (pos != settings.end())
                D_att_z[i] = std::stod(pos->second);
        }

        //C_f_w
        for (int i=0; i < C_f_w.size(); i++ ){
            C_f_w[i] = 0.;
            pos = settings.find("C_f_w["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_f_w[i] = std::stod(pos->second);
        }

        //C_f_rot
        for (int i=0; i < C_f_rot.size(); i++ ){
            C_f_rot[i] = 0.;
            pos = settings.find("C_f_rot["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_f_rot[i] = std::stod(pos->second);
        }

        //C_f_att_d
        for (int i=0; i < C_f_att_d.size(); i++ ){
            C_f_att_d[i] = 0.;
            pos = settings.find("C_f_att_d["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_f_att_d[i] = std::stod(pos->second);
        }

        //C_f_ali_d
        for (int i=0; i < C_f_ali_d.size(); i++ ){
            C_f_ali_d[i] = 0.;
            pos = settings.find("C_f_ali_d["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_f_ali_d[i] = std::stod(pos->second);
        }

        //C_f_att_z_d
        for (int i=0; i < C_f_att_z_d.size(); i++ ){
            C_f_att_z_d[i] = 0.;
            pos = settings.find("C_f_att_z_d["+ std::to_string(i) + "]");
            if (pos != settings.end())
                C_f_att_z_d[i] = std::stod(pos->second);
        }
    }
}

void createSimulator(){
    cerr<<"Creating simulator ..."<<endl;

    sim = new Simulator();

    if (!NETWORK_TIME){
        sim->setDt(DT);
    }

    //Create tank
    if (FISHTANK){
        sim->addTank(new FishTank(0.0, 0.0, 26, 0));
    }

    //Create fish
    if (NB_FISH_REPLAY>0){

        for (int i=0; i<NB_FISH_REPLAY; i++){
            Fish *f = new Fish();
            ReplayController* newCtrl = new ReplayController();
            newCtrl->linkSimulator(sim);
            f->setController(newCtrl);
            sim->addFish(f);
        }

        cerr<<"Starting positions file parsing"<<endl;
        Parser::parseReplayData(REPLAY_FILE, sim);
        cerr<<"Parsing positions file done"<<endl;

        if (SHOW_NEIGHBORS){
            cerr<<"Starting neighbors file parsing"<<endl;
            Parser::parseNeighborsData(NEIGHBORS_FILE, sim, K_NEIGHBORS);
            cerr<<"Parsing neighbors file done"<<endl;
        }
    }

    if (NB_FISH_NETWORK>0){
        for (int i=0; i<NB_FISH_NETWORK; i++){
          Fish* f = new Fish();
          NetworkController* newCtrl = new NetworkController();
          newCtrl->linkSimulator(sim);
          f->setController(newCtrl);
          sim->addFish(f);
        }
    }

    if(NB_FISH_CIRCLE>0){
        for (int i=0; i<NB_FISH_CIRCLE; i++){
          Fish* f = new Fish();
          CircularController* newCtrl = new CircularController();
          newCtrl->linkSimulator(sim);
          f->setController(newCtrl);
          sim->addFish(f);
        }
    }

    if(NB_FISH_ROSACE>0){
        for (int i=0; i<NB_FISH_ROSACE; i++){
            Fish* f = new Fish();
            RosaceController* newCtrl = new RosaceController();
            newCtrl->linkSimulator(sim);
            f->setController(newCtrl);
            sim->addFish(f);
        }
    }

    if(NB_FISH_FIXED>0){
        for (int i=0; i<NB_FISH_FIXED; i++){
          Fish* f = new Fish();
          FixedPositionController* newCtrl = new FixedPositionController();
          newCtrl->linkSimulator(sim);
          f->setController(newCtrl);
          sim->addFish(f);
        }
    }

    if(NB_FISH_2D_MODEL>0){
        for (int i=0; i<NB_FISH_2D_MODEL; i++){
          Fish* f = new Fish();
          Controller2D* newCtrl = new Controller2D();
          newCtrl->model_mode = model_mode;
          newCtrl->linkSimulator(sim);
          f->setController(newCtrl);
          sim->addFish(f);
        }
    }

    if(NB_FISH_3D_MODEL>0){
        // Read parameters
        readParametersFile();

        if (C_v_0>-1.){
            C_v[0] = C_v_0;
        }
        if (C_f_z_0>-1.){
            C_f_z[0] = C_f_z_0;
        }
        if(C_f_att_d_1>-1){
            C_f_att_d[1] = C_f_att_d_1;
        }

        for (int i=0; i<NB_FISH_3D_MODEL; i++){
            Fish* f = new Fish();
            Controller3D* newCtrl = new Controller3D();
            newCtrl->linkSimulator(sim);
            f->setController(newCtrl);
            sim->addFish(f);
        }
    }

    NB_FISH = (int)(sim->fish.size());

    for (int i=0;i<NB_FISH; i++){
        sim->fish[i]->init();
    }

    /*
    for (int i=0;i<NB_FISH; i++){
        sim->fish[i]->update();
    }
    */

    //sim->computeFishData();

    cout<<"... simulator created !"<<endl;
}

void readConfigurationFile(){
    std::cerr<<"Reading configuration file ..."<<std::endl<<std::flush;
    // std::ifstream is RAII, i.e. no need to call close
    std::ifstream cFile ("config.txt");
    std::map<std::string, std::string> settings;
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            std::string name = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            //std::cerr << name << " " << value << '\n'<<std::endl<<std::flush;
            settings[name] = value;
        }

        std::map<std::string, std::string>::const_iterator pos;

        pos = settings.find("data_folder");
        if (pos != settings.end())
            dataFolder = pos->second;

        pos = settings.find("writing_fish_data");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                writeData = true;
            else
                writeData = false;
        }

        pos = settings.find("writing_pictures");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                writePictures = true;
            else
                writePictures = false;
        }

        pos = settings.find("writing_pdf");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                writePDF = true;
            else
                writePDF = false;
        }

        pos = settings.find("writing_trajectories");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                writeTrajectories = true;
            else
                writeTrajectories = false;
        }

        pos = settings.find("dt");
        if (pos != settings.end())
            DT = std::stod(pos->second);

        pos = settings.find("sim_time");
        if (pos != settings.end())
            SIM_TIME = std::stoi(pos->second);

        pos = settings.find("t_min");
        if (pos != settings.end())
            T_MIN = std::stoi(pos->second);

        pos = settings.find("network_time");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                NETWORK_TIME = true;
            else
                NETWORK_TIME = false;
        }

        pos = settings.find("nb_fish_replay");
        if (pos != settings.end())
            NB_FISH_REPLAY = std::stoi(pos->second);

        pos = settings.find("nb_fish_circle");
        if (pos != settings.end())
            NB_FISH_CIRCLE = std::stoi(pos->second);

        pos = settings.find("nb_fish_rosace");
        if (pos != settings.end())
            NB_FISH_ROSACE = std::stoi(pos->second);

        pos = settings.find("nb_fish_fixed");
        if (pos != settings.end())
            NB_FISH_FIXED = std::stoi(pos->second);

        pos = settings.find("nb_fish_2D_model");
        if (pos != settings.end())
            NB_FISH_2D_MODEL = std::stoi(pos->second);

        pos = settings.find("nb_fish_3D_model");
        if (pos != settings.end())
            NB_FISH_3D_MODEL = std::stoi(pos->second);

        pos = settings.find("nb_fish_network");
        if (pos != settings.end())
            NB_FISH_NETWORK = std::stoi(pos->second);

        pos = settings.find("replay_file");
        if (pos != settings.end())
            REPLAY_FILE = pos->second;

        pos = settings.find("graphic_mode");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                GRAPHIC_MODE = true;
            else
                GRAPHIC_MODE = false;
        }

        pos = settings.find("window_width");
        if (pos != settings.end())
            WINDOW_WIDTH = stoi(pos->second);

        pos = settings.find("window_height");
        if (pos != settings.end())
            WINDOW_HEIGHT = stoi(pos->second);

        pos = settings.find("world_to_pixel");
        if (pos != settings.end())
            WORLD_TO_PIXEL = stoi(pos->second);

        pos = settings.find("antialiasing");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                ANTIALIASING = true;
            else
                ANTIALIASING = false;
        }

        pos = settings.find("grid");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                GRID = true;
            else
                GRID = false;
        }

        pos = settings.find("grid_square_size");
        if (pos != settings.end())
            GRID_SQUARE_SIZE = stoi(pos->second);

        pos = settings.find("background");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                BACKGROUND = true;
            else
                BACKGROUND = false;
        }

        pos = settings.find("dark_background");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                DARK_BACKGROUND = true;
            else
                DARK_BACKGROUND = false;
        }

        pos = settings.find("fishtank");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                FISHTANK = true;
            else
                FISHTANK = false;
        }

        pos = settings.find("show_fish");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                SHOW_FISH = true;
            else
                SHOW_FISH = false;
        }

        pos = settings.find("white_fish");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                WHITE_FISH = true;
            else
                WHITE_FISH = false;
        }

        pos = settings.find("show_trajectory");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                SHOW_TRAJECTORY = true;
            else
                SHOW_TRAJECTORY = false;
        }

        pos = settings.find("trajectory_length");
        if (pos != settings.end())
            TRAJECTORY_LENGTH = stoi(pos->second);

        pos = settings.find("trajectory_width");
        if (pos != settings.end())
            TRAJECTORY_WIDTH = stoi(pos->second);

        pos = settings.find("moving_camera");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                MOVING_CAMERA = true;
            else
                MOVING_CAMERA = false;
        }

        pos = settings.find("init_camera_at_distrib_center");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                INIT_CAMERA_AT_DISTRIB_CENTER = true;
            else
                INIT_CAMERA_AT_DISTRIB_CENTER = false;
        }

        pos = settings.find("follow_fish");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                FOLLOW_FISH = true;
            else
                FOLLOW_FISH = false;
        }

        pos = settings.find("focused_fish");
        if (pos != settings.end())
            FOCUSED_FISH = stoi(pos->second);

        pos = settings.find("show_neighbors");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                SHOW_NEIGHBORS = true;
            else
                SHOW_NEIGHBORS = false;
        }

        pos = settings.find("k_neighbors");
        if (pos != settings.end())
            K_NEIGHBORS = stoi(pos->second);

        pos = settings.find("neighbors_file");
        if (pos != settings.end())
            NEIGHBORS_FILE = pos->second;

        pos = settings.find("neighbors_focal_fish");
        if (pos != settings.end())
            NEIGHBORS_FOCAL_FISH = stoi(pos->second);

        pos = settings.find("neighbors_max_frames");
        if (pos != settings.end())
            NEIGHBORS_MAX_FRAMES = stoi(pos->second);

        pos = settings.find("fix_neighbors_indexes");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                FIX_NEIGHBORS_INDEXES = true;
            else
                FIX_NEIGHBORS_INDEXES = false;
        }

        pos = settings.find("arena_radius");
        if (pos != settings.end())
            arena_radius = std::stod(pos->second);

        /*------------------ Circle controller -------------------*/

        pos = settings.find("circle_fish_ref_id");
        if (pos != settings.end())
            circle_referencefishid = std::stoi(pos->second);

        pos = settings.find("circle_radius");
        if (pos != settings.end())
            circle_radius = std::stod(pos->second);

        pos = settings.find("circle_fixedz");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                circle_fixedz = true;
            else
                circle_fixedz = false;
        }

        pos = settings.find("circle_reverse");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                circle_reverse = true;
            else
                circle_reverse = false;
        }

        pos = settings.find("circle_zmean");
        if (pos != settings.end())
            circle_zmean = std::stod(pos->second);

        pos = settings.find("circle_bc");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                circle_bc = true;
            else
                circle_bc = false;
        }

        pos = settings.find("circle_v0");
        if (pos != settings.end())
            circle_v0 = std::stod(pos->second);

        pos = settings.find("circle_tau0");
        if (pos != settings.end())
            circle_tau0 = std::stod(pos->second);

        pos = settings.find("circle_taumean");
        if (pos != settings.end())
            circle_taumean = std::stod(pos->second);

        pos = settings.find("circle_taumin");
        if (pos != settings.end())
            circle_taumin = std::stod(pos->second);

        pos = settings.find("circle_vmean");
        if (pos != settings.end())
            circle_vmean = std::stod(pos->second);

        pos = settings.find("circle_vmin");
        if (pos != settings.end())
            circle_vmin = std::stod(pos->second);

        pos = settings.find("circle_vcut");
        if (pos != settings.end())
            circle_vcut = std::stod(pos->second);

        /*------------------ Rosace controller -------------------*/

        pos = settings.find("rosace_L");
        if (pos != settings.end())
            rosace_L = std::stod(pos->second);

        pos = settings.find("rosace_p");
        if (pos != settings.end())
            rosace_p = std::stod(pos->second);

        pos = settings.find("rosace_q");
        if (pos != settings.end())
            rosace_q = std::stod(pos->second);

        pos = settings.find("rosace_v0");
        if (pos != settings.end())
            rosace_v0 = std::stod(pos->second);

        pos = settings.find("rosace_vmean");
        if (pos != settings.end())
            rosace_vmean = std::stod(pos->second);

        pos = settings.find("rosace_fixedz");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                rosace_fixedz = true;
            else
                rosace_fixedz = false;
        }

        pos = settings.find("rosace_zmean");
        if (pos != settings.end())
            rosace_zmean = std::stod(pos->second);

        pos = settings.find("rosace_reverse");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                rosace_reverse = true;
            else
                rosace_reverse = false;
        }

        /*------------------ Fixed position controller -------------------*/

        pos = settings.find("fixed_fish_ref_id");
        if (pos != settings.end())
            fixed_referencefishid = std::stoi(pos->second);

        pos = settings.find("fixed_fixedz");
        if (pos != settings.end()){
            if (pos->second.compare("true")==0)
                fixed_fixedz = true;
            else
                fixed_fixedz = false;
        }

        pos = settings.find("fixed_zmean");
        if (pos != settings.end())
            fixed_zmean = std::stod(pos->second);

        pos = settings.find("fixed_distance");
        if (pos != settings.end())
            fixed_distance = std::stod(pos->second);

        pos = settings.find("fixed_angle");
        if (pos != settings.end())
            fixed_angle = std::stod(pos->second);

        /*--------------- 2D Model -------------------------------*/

        pos = settings.find("model_mode");
        if (pos != settings.end())
            model_mode = std::stoi(pos->second);

        pos = settings.find("z0");
        if (pos != settings.end())
            z0 = std::stod(pos->second);

        pos = settings.find("zmean");
        if (pos != settings.end())
            zmean = std::stod(pos->second);

        pos = settings.find("tau0");
        if (pos != settings.end())
            tau0 = std::stod(pos->second);

        pos = settings.find("taumean");
        if (pos != settings.end())
            taumean = std::stod(pos->second);

        pos = settings.find("taumin");
        if (pos != settings.end())
            taumin = std::stod(pos->second);

        pos = settings.find("vmean");
        if (pos != settings.end())
            vmean = std::stod(pos->second);

        pos = settings.find("vmin");
        if (pos != settings.end())
            vmin = std::stod(pos->second);

        pos = settings.find("vcut");
        if (pos != settings.end())
            vcut = std::stod(pos->second);

        pos = settings.find("gamma_rand");
        if (pos != settings.end())
            gamma_rand = std::stod(pos->second);

        pos = settings.find("gamma_wall");
        if (pos != settings.end())
            gamma_wall = std::stod(pos->second);

        pos = settings.find("gamma_attraction");
        if (pos != settings.end())
            gamma_attraction = std::stod(pos->second);

        pos = settings.find("gamma_alignement");
        if (pos != settings.end())
            gamma_alignement = std::stod(pos->second);

        pos = settings.find("gamma_sym");
        if (pos != settings.end())
            gamma_sym = std::stod(pos->second);

        pos = settings.find("gamma_asym");
        if (pos != settings.end())
            gamma_asym = std::stod(pos->second);

        pos = settings.find("L_ali");
        if (pos != settings.end())
            L_ali = std::stod(pos->second);

        pos = settings.find("L_att");
        if (pos != settings.end())
            L_att = std::stod(pos->second);

        pos = settings.find("A");
        if (pos != settings.end())
            A = std::stod(pos->second);

        pos = settings.find("C");
        if (pos != settings.end())
            C = std::stod(pos->second);

        pos = settings.find("gamma_r_chi");
        if (pos != settings.end())
            gamma_r_chi = std::stod(pos->second);

        pos = settings.find("gamma_par_chi");
        if (pos != settings.end())
            gamma_par_chi = std::stod(pos->second);

        pos = settings.find("gamma_per_chi");
        if (pos != settings.end())
            gamma_per_chi = std::stod(pos->second);

        pos = settings.find("gamma_z");
        if (pos != settings.end())
            gamma_z = std::stod(pos->second);

        pos = settings.find("gamma_z_par");
        if (pos != settings.end())
            gamma_z_par = std::stod(pos->second);

        pos = settings.find("l_w");
        if (pos != settings.end())
            l_w = std::stod(pos->second);

        pos = settings.find("epsilon_2");
        if (pos != settings.end())
            epsilon_2 = std::stod(pos->second);

        pos = settings.find("dw");
        if (pos != settings.end())
            dw = std::stod(pos->second);

        pos = settings.find("dc");
        if (pos != settings.end())
            dc = std::stod(pos->second);

        pos = settings.find("alpha_w");
        if (pos != settings.end())
            alpha_w = std::stod(pos->second);

        pos = settings.find("max_kick_computation_tentatives");
        if (pos != settings.end())
            max_kick_computation_tentatives = std::stoi(pos->second);

        pos = settings.find("vmem");
        if (pos != settings.end())
            vmem = std::stod(pos->second);

        pos = settings.find("vmem_12");
        if (pos != settings.end())
            vmem_12 = std::stod(pos->second);

        /*--------------- 3D Model -------------------------------*/

        pos = settings.find("3D_param_file");
        if (pos != settings.end())
            param_file = pos->second;

        pos = settings.find("K");
        if (pos != settings.end())
            K = std::stoi(pos->second);

        pos = settings.find("renorm_K_pow");
        if (pos != settings.end())
            renorm_K_pow = std::stod(pos->second);

        pos = settings.find("sigma_para");
        if (pos != settings.end())
            sigma_para = std::stod(pos->second);

        pos = settings.find("sigma_perp");
        if (pos != settings.end())
            sigma_perp = std::stod(pos->second);

        pos = settings.find("tau_XY");
        if (pos != settings.end())
            tau_XY = std::stod(pos->second);

        pos = settings.find("sigma_Z");
        if (pos != settings.end())
            sigma_Z = std::stod(pos->second);

        pos = settings.find("tau_Z");
        if (pos != settings.end())
            tau_Z = std::stod(pos->second);

        pos = settings.find("C_v_0");
        if (pos != settings.end())
            C_v_0 = std::stod(pos->second);

        pos = settings.find("C_f_z_0");
        if (pos != settings.end())
            C_f_z_0 = std::stod(pos->second);

        pos = settings.find("C_f_att_d_1");
        if (pos != settings.end())
            C_f_att_d_1 = std::stod(pos->second);

        pos = settings.find("Coef_Adapt");
        if (pos != settings.end())
            Coef_Adapt = std::stod(pos->second);

        pos = settings.find("Coef_Friction");
        if (pos != settings.end())
            Coef_Friction = std::stod(pos->second);

        pos = settings.find("Coef_Friction_z");
        if (pos != settings.end())
            Coef_Friction_z = std::stod(pos->second);

        pos = settings.find("Coef_f_w");
        if (pos != settings.end())
            Coef_f_w = std::stod(pos->second);

        pos = settings.find("Coef_f_rot");
        if (pos != settings.end())
            Coef_f_rot = std::stod(pos->second);

        pos = settings.find("Coef_f_z");
        if (pos != settings.end())
            Coef_f_z = std::stod(pos->second);

        pos = settings.find("Coef_f_att");
        if (pos != settings.end())
            Coef_f_att = std::stod(pos->second);

        pos = settings.find("Coef_f_ali");
        if (pos != settings.end())
            Coef_f_ali = std::stod(pos->second);

        pos = settings.find("Coef_f_att_z");
        if (pos != settings.end())
            Coef_f_att_z = std::stod(pos->second);

        pos = settings.find("No_Int");
        if (pos != settings.end())
            No_Int = std::stoi(pos->second);

        pos = settings.find("No_Wall");
        if (pos != settings.end())
            No_Wall = std::stoi(pos->second);

        pos = settings.find("No_Wall_z");
        if (pos != settings.end())
            No_Wall_z = std::stoi(pos->second);

        pos = settings.find("d_c");
        if (pos != settings.end())
            d_c = std::stod(pos->second);

        pos = settings.find("dBounce");
        if (pos != settings.end())
            dBounce = std::stod(pos->second);

        pos = settings.find("f_c");
        if (pos != settings.end())
            f_c = std::stod(pos->second);

        pos = settings.find("d_c_att");
        if (pos != settings.end())
            d_c_att = std::stod(pos->second);

        pos = settings.find("h_water");
        if (pos != settings.end())
            h_water = std::stod(pos->second);

        pos = settings.find("geometry");
        if (pos != settings.end())
            geometry = pos->second;

        pos = settings.find("idum");
        if (pos != settings.end())
            idum = std::stol(pos->second);

        cerr<<"... configuration file read !"<<endl;
    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
    }
}

void createFoldersAndFiles(){
    std::cerr  << "Creating folder and file ..."<<endl;

    //Get date string
    time_t curr_time;
    tm * curr_tm;
    char date_string[100];

    time(&curr_time);
    curr_tm = localtime(&curr_time);
    strftime(date_string, 100, "%Y%m%d_%H%M%S", curr_tm);

    //Create folders
    stringstream ssfd;
    ssfd << "./Data/" << date_string << "_" << NB_FISH << "Fish/";
    foldername = ssfd.str();


    stringstream ssfl;
    ssfl << date_string << "_" << NB_FISH << "fish";
    filename = ssfl.str();

    stringstream dataname;
    dataname << foldername << filename << ".txt";
    WORLD_FILE = dataname.str();

    _mkdir(foldername.c_str());

    ofstream datafile(dataname.str().c_str());
    datafile.close();

    std::cerr  << "... folder and file created !"<<endl;
}


void processOneStep(){
    if (!sim->pause)
    {
        //cout<<sim->simSteps<<endl;
        //sim->pause = false;

        if (sim->simulationFinished())
        {
            cout << "Finished !" << endl;
            cout << "# icpt = " << sim->nbIcpt << endl;
            cout << "Time of finish: " << sim->actualTime << endl;
            sim->pause = true;
        }
        sim->oneStep();
    }
    else
    {
        if (sim->simulationFinished())
        {
            cout << "Finished !" << endl;
            cout << "# icpt = " << sim->nbIcpt << endl;
            cout << "Time of finish: " << sim->actualTime << endl;
            sim->pause = true;
            //pause = true;

            //QApplication::quit();
            //exit(0);
        }
    }
}


int main(int argc, char *argv[])
{
    //versionSelection(argc, argv);

    readConfigurationFile();

    createSimulator();

    dm = new DataManager(sim);
    dm->createFoldersAndFiles();

    if (NB_FISH_3D_MODEL < 1){
        writePDF = false;
        dm->writepdf = false;
    }

    QApplication a(argc, argv);

    nm = new NetworkManager(sim);


    if (GRAPHIC_MODE) {
        cerr<<"Starting program"<<endl;

        static double seed = 42;
        srand(seed);

        cerr<<"Starting Main Window"<<endl;

        MainWindow w(WINDOW_WIDTH, WINDOW_HEIGHT, WORLD_TO_PIXEL, 0, sim, dm, nm);
        
        w.show();

        int v =  a.exec();
 
        return v;
    }
    else {
        clock_t t0, t1;
        t0 = clock();

        sim->pause = false;

        std::cout<<"Start simulation ..."<<endl;

        while (! sim->simulationFinished()){
            processOneStep();
            dm->writeFishData();
            dm->computePDF();
            dm->computeTrajectories();
        }

        dm->savePDF();
        dm->saveTrajectories();

        std::cout<< "... simulation done !"<<endl;

        t1 = clock();
        double runTime = (double) (t1 - t0);
        std::cout<<"Execution time : " << runTime/CLOCKS_PER_SEC<<"s"<<endl;

        return 1;
    }
}


