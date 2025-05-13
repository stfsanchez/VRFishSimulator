/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <iostream>

#include <QtWidgets/qwidget.h>
#include "Core/simulator.h"

class QUdpSocket;

class UdpSender : public QWidget
{
public:
    int port;

    Simulator *sim;

    explicit UdpSender(QWidget *parent = nullptr);

    void init();

    void linkSim(Simulator *pSim);

    void writeDatagram();

private slots:


private:
    QUdpSocket *udpSocket = nullptr;


};

#endif // UDPRECEIVER_H
