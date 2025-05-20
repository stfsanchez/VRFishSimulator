/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "udpreceiver.h"

#include <string>
#include <iostream>
#include "utils.h"
#include <QtNetwork/QUdpSocket>

#include "Agent/networkcontroller.h"
#include "globalconstants.h"
#include "mainwindow.h"

using namespace std;

UdpReceiver::UdpReceiver(QWidget *parent)
    : QWidget(parent)
{
    this->sim = NULL;
    this->once = false;
}

void UdpReceiver::linkSim(Simulator *pSim){
    this->sim = pSim;
}

void UdpReceiver::init(){
    udpSocket = new QUdpSocket(this);
    //udpSocket->bind(QHostAddress::LocalHost, 65432);
    udpSocket->bind(QHostAddress(IP_IN.c_str()), PORT_IN);

    connect(udpSocket, &QUdpSocket::readyRead,this, &UdpReceiver::readPendingDatagrams);
    cout<<"Receiver init done !"<<std::endl;
}

void UdpReceiver::readPendingDatagrams()
{
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams()) {
            datagram.resize(int(udpSocket->pendingDatagramSize()));
            udpSocket->readDatagram(datagram.data(), datagram.size());
            processDatagram(datagram);
    }
}

void UdpReceiver::processDatagram(QByteArray datagram){
    string line = datagram.data();

    //cout<<line<<endl;

    /* Process the line  */
    vector<string> tokens;
    tokens = split(line," ");
    if (tokens.size()>4){
        //Remove 4 first tokens
        tokens.erase(tokens.begin()); // number of fish
        WORLD_FILE = tokens[0];    //World file name
        tokens.erase(tokens.begin());
        FRAME_ID = atof(tokens[0].c_str()); //Frame id
        tokens.erase(tokens.begin());
        if (NETWORK_TIME){
            this->sim->setActualTime(atof(tokens[0].c_str()));
        }
        tokens.erase(tokens.begin());

        if (this->once){
            unsigned int first = (unsigned int)NB_FISH_REPLAY;

            //Add data to controller for each fish
            for (unsigned int f=0; f<(unsigned int)NB_FISH_NETWORK; f++){
                    Point np = Point(atof(tokens[f*5].c_str()), atof(tokens[f*5+1].c_str()), atof(tokens[f*5+2].c_str()));
                    if (sim->fish.size()>0){
                        ((NetworkController*)(sim->fish[first+f]->ctrl))->addPosition(np);
                        ((NetworkController*)(sim->fish[first+f]->ctrl))->addPhi(atof(tokens[f*5+3].c_str()));
                        ((NetworkController*)(sim->fish[first+f]->ctrl))->addChi(atof(tokens[f*5+4].c_str()));
                    }
            }
        }
    }

    if (NETWORK_TIME && this->once){
        ((MainWindow*)this->parent())->processOneStep();
    }

    this->once = true;
}
