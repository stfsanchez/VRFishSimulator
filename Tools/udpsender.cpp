/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "udpsender.h"

#include <string>
#include <iostream>
#include <sstream>
#include <QtNetwork/QUdpSocket>

#include "globalconstants.h"

using namespace std;

UdpSender::UdpSender(QWidget *parent)
    : QWidget(parent)
{
    this->sim = NULL;
    this->port = PORT_OUT;
}

void UdpSender::linkSim(Simulator *pSim){
    this->sim = pSim;
}

void UdpSender::init(){
    udpSocket = new QUdpSocket(this);
    //udpSocket->bind(QHostAddress::LocalHost, 65435);
    //udpSocket->bind(QHostAddress(IP_OUT.c_str()), PORT_OUT);

    cout<<"Sender to port "<< this->port<<" init done !"<<std::endl;
}

void UdpSender::writeDatagram(){
    //QByteArray datagram;
    stringstream out;
    //QDataStream out(&datagram, QIODevice::WriteOnly);

    out.precision(8);

    out << this->sim->fish.size() <<" ";
    out << WORLD_FILE.c_str() << " ";
    out <<std::fixed<<this->sim->actualTime;
    for (unsigned int i=0; i<this->sim->fish.size(); i++){
        out << " " << this->sim->fish[i]->toString().c_str();
    }
    out << " \n";

    //out.setVersion(QDataStream::Qt_4_3);
    const std::string& tmp = out.str();
    const char* cstr = tmp.c_str();

    udpSocket->writeDatagram(cstr, QHostAddress(IP_OUT.c_str()), this->port);

    out.flush();
}


