/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "Core/simulator.h"

#include <string>
#include <QPixmap>

using namespace std;

class DataManager
{
public:
    Simulator *sim;

    bool writedata;
    bool writeimg;
    bool writepdf;
    bool writetrajectories;

    string foldername;
    string filename;
    string picturesfoldername;

    int Symmetrize;
    int it_min = 0;
    int it_max = 0;
    int it_traj = 0;
    int it_dt_traj = 0;
    int it_start = 0;
    int interval_traj = 0;
    int itest_traj = 0;

    DataManager(Simulator *nsim);

    void setWriteFlags(bool writedata, bool writeimg);

    void createFoldersAndFiles();

    void writeFishData();

    void writePicture(QPixmap pix, int imgfilecount);

    void computePDF();

    void computeRealFishPDF();

    void computeTrajectories();

    void savePDF();

    void saveRealFishPDF();

    void saveTrajectories();
};

#endif // DATAMANAGER_H
