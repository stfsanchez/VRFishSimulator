/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <iostream>

#include <QtWidgets/qwidget.h>
#include "Core/simulator.h"

class QUdpSocket;

class UdpReceiver : public QWidget
{
    public:
        Simulator *sim;

        bool once;

        //udpReceiver();

        explicit UdpReceiver(QWidget *parent = nullptr);

        void init();

        void linkSim(Simulator *pSim);

    private slots:
        void processPendingDatagrams();

    private:
        QUdpSocket *udpSocket = nullptr;

        void readPendingDatagrams();

        void processDatagram(QByteArray datagram);


};

#endif // UDPRECEIVER_H
