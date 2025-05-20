/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "parser.h"
#include "utils.h"
#include "globalconstants.h"

#include "Agent/controller.h"
#include "Agent/replaycontroller.h"
#include "Agent/fish.h"

#include <iostream>
#include <direct.h>

using namespace std;

void Parser::parseReplayData(string filepath, Simulator* sim)
{
    // Add controllers to fish
    for (unsigned int i=0; i<sim->fish.size(); i++){
        if (sim->fish[i]->ctrl != NULL){
            delete sim->fish[i]->ctrl;
        }
        ReplayController* newCtrl = new ReplayController();
        newCtrl->linkSimulator(sim);
        sim->fish[i]->setController(newCtrl);
    }

    /*------------ Handling of replay file : fish positions ------------------------------*/
    char * dirpath = NULL;
    dirpath = _getcwd(NULL, 0); // or _getcwd
    string fullFilepath = string(dirpath);
    fullFilepath = fullFilepath + "/Data/" + filepath;

    // Opening of the file to parse
    ifstream file;
    file.open(fullFilepath.c_str());


    // Test if the file is open
    if (!file.is_open()) {
        cerr << "File: "<<fullFilepath<<" not found" << endl;
        return;
    }

    // Loop handling the parsing of the file
    string line = "";
    while (!file.eof()){
        getline(file,line);
        //cout << "Reading line : [" << line << "]" << endl;

        /* Process the line  */
        vector<string> tokens;
        tokens = split(line," ");
        if (tokens.size()>0){
            //Remove timestamp (first token)
            tokens.erase(tokens.begin());

            //Add data to controller for each fish
            for (unsigned int f=0; f<sim->fish.size(); f++){
                    Point np = Point(atof(tokens[f*3].c_str()), atof(tokens[f*3+1].c_str()), 0);
                    ((ReplayController*)(sim->fish[f]->ctrl))->addPosition(np);
                    ((ReplayController*)(sim->fish[f]->ctrl))->addDirection(atof(tokens[f*3+2].c_str()));
            }
        }
    }

    //Initialize fish
    for (unsigned int f=0; f<sim->fish.size(); f++){
        sim->fish[f]->x = ((ReplayController*)(sim->fish[f]->ctrl))->positions[0].x*100; //cm
        sim->fish[f]->y = ((ReplayController*)(sim->fish[f]->ctrl))->positions[0].y*100; //cm
        sim->fish[f]->phi = ((ReplayController*)(sim->fish[f]->ctrl))->directions[0];
    }
}

void Parser::parseNeighborsData(string filepath, Simulator* sim, int k){
    // Check controllers to fish
    for (unsigned int i=0; i<sim->fish.size(); i++){
        if (sim->fish[i]->ctrl == NULL){
            ReplayController* newCtrl = new ReplayController();
            newCtrl->linkSimulator(sim);
            sim->fish[i]->setController(newCtrl);
        }
    }

    /*------------ Handling of replay file : fish positions ------------------------------*/
    char * dirpath = NULL;
    dirpath = _getcwd(NULL, 0); // or _getcwd
    string fullFilepath = string(dirpath);
    fullFilepath = fullFilepath + "/Data/" + filepath;

    // Opening of the file to parse
    ifstream file;
    file.open(fullFilepath.c_str());

    // Test if the file is open
    if (!file.is_open()) {
        cerr << "File: "<<fullFilepath<<" not found" << endl;
        return;
    }

    // Loop handling the parsing of the file
    string line = "";
    while (!file.eof()){
        getline(file,line);
        //cout << "Reading line : [" << line << "]" << endl;

        /* Process the line  */
        vector<string> tokens;
        tokens = split(line," ");
        if (tokens.size()>0){
            //Remove timestamp (first token)
            tokens.erase(tokens.begin());

            //Add data to controller for each fish
            for (unsigned int f=0; f<sim->fish.size(); f++){
                    vector<int> n;
                    for (int i=0; i<k; i++){
                        int index = atof(tokens[(f*k)+i].c_str());
                        if (FIX_NEIGHBORS_INDEXES && index > 0){
                            index--;
                        }
                        n.push_back(index);
                    }
                    ((ReplayController*)(sim->fish[f]->ctrl))->addNeighbors(n);
            }
        }
    }

    //Initialize fish
    for (unsigned int f=0; f<sim->fish.size(); f++){
        //set k (number of influencial neighbors)
        sim->fish[f]->k = k;

        // set first neighbors
        sim->fish[f]->neighbors = ((ReplayController*)(sim->fish[f]->ctrl))->neighbors[0];
    }
}

