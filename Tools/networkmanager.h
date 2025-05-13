/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Core/simulator.h"
#include "udpreceiver.h"
#include "udpsender.h"

using namespace std;

class NetworkManager
{
public:
    Simulator *sim;

    UdpReceiver *receiver;
    UdpSender *sender;
    UdpSender *toWriter;

    NetworkManager(Simulator * sim);
};

#endif // NETWORKMANAGER_H
