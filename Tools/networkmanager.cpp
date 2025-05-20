/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "networkmanager.h"

#include "globalconstants.h"

NetworkManager::NetworkManager(Simulator * nsim) {
    this->sim = nsim;

    receiver = new UdpReceiver();
    receiver->linkSim(this->sim);
    receiver->init();

    sender = new UdpSender();
    sender->port = PORT_OUT;
    sender->linkSim(this->sim);
    sender->init();

    toWriter = new UdpSender();
    toWriter->port = 65434;
    toWriter->linkSim(this->sim);
    toWriter->init();
}
