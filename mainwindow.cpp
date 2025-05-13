/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "globalconstants.h"
//#include "parser.h"
#include "Agent/fish.h"
#include "Graphics/spritefish.h"
#include "Graphics/spritetank.h"
#include "Graphics/spriteplainbackground.h"
//#include "Graphics/spritecenter.h"
#include "Graphics/spritegrid.h"

#include "Agent/circularcontroller.h"
#include "Agent/rosacecontroller.h"
#include "Agent/controller2dconstants.h"
#include "Agent/controller3dconstants.h"

#include <QtGui/QWindow>
#include <QtGui/QScreen>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QApplication>


#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <direct.h>

using namespace std;


MainWindow::MainWindow(int width, int height, int scaling, QWidget *parent, Simulator* nsim, DataManager * dm, NetworkManager * nm) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  //Window geometry, size of pictures
  window_width = width;
  window_height = height;

  //Scaling factor - conversion from meter to pixel
  //Zoom in or out (lesser is out)
  world_to_pixel = scaling;

  ui->setupUi(this);

  //this->setFixedSize(window_width + 360, window_height);
  //this->setFixedSize(window_width, window_height);
  //ui->verticalLayoutWidget_2->setGeometry(window_width, 0, 200, window_height);
  //ui->vl3DModel->setGeometry(window_width+200, 0, 160, window_height);
  //ui->imglabel->setGeometry(0, 0, window_width, window_height);

  sim = nsim;
  sim->pause = true;

  vm = new ViewModifier();
  vm->setWorldToPixel(world_to_pixel);
  vm->setWindow(window_width/2, window_height/2, window_width, window_height);
  vm->setMovingCamera(MOVING_CAMERA);
  vm->setFollowFish(FOLLOW_FISH, FOCUSED_FISH);
  vm->setInitCenter(INIT_CAMERA_AT_DISTRIB_CENTER);
  vm->setSimulator(sim);

  int dt_tmp = (int)(DT*1000);
  timer = new QTimer(this);
  timer->setInterval(dt_tmp);
  timer->stop();

  if (!NETWORK_TIME){
    connect(timer, SIGNAL(timeout()), this, SLOT(onestep()));
  }

  /** Colors **/
  colors = {QColor(245, 137, 173, 255), QColor(137, 148, 245, 255), QColor(137, 245, 196, 255), QColor(153, 245, 137, 255), QColor(253, 230, 133, 255),
            QColor(0,200,155,255), QColor(49,163,84,255), QColor(120,198,121,255), QColor(194,230,153,255), QColor(200,255,204,255)} ;

  focusedColor = QColor(255, 0, 0, 255);
  standardColor = QColor(192, 192, 192, 255);
  neighborsColors = {QColor(0, 0, 204, 255), QColor(0, 128, 255, 255), QColor(153, 204, 255, 255)};

  this->dm = dm;

  firstStep = true;

  frameTmp = 0;
  frameTresh = 1;
  frameCount = 0;

  if (sim != NULL){
    load();
    this->move(0,0);
    this->initBoxes();
    this->initSliders();
    this->initCircularControls();
    this->initRosaceControls();
    this->init3DModelControls();
  }

  this->nm = nm;

  if (!NETWORK_TIME){
    timer->start();
  }
}


MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::paintEvent(QPaintEvent * event)
{
  if (frameTmp < frameTresh) {

    QPixmap pix(window_width, window_height);
    QPainter painter(&pix);

    if (ANTIALIASING){
        painter.setRenderHint(QPainter::Antialiasing, true);
    }

    //for (unsigned int i = 0; i < sprites.size(); i++) {
    //  sprites[i]->draw(&painter, vm, frameTmp);
    //}

    for(std::map<std::string, Sprite *>::iterator it = sprites.begin(); it != sprites.end(); ++it){
        it->second->draw(&painter, vm, frameTmp);
    }

    if (SHOW_NEIGHBORS){
        //Draw all fish except focal and its neighbors
        for (unsigned int i = 0; i < spritesFish.size(); i++) {
          spritesFish[i]->setColor(standardColor);
          spritesFish[i]->setTrajectoryColor(standardColor);
          spritesFish[i]->setDirectionColor(Qt::darkRed);
          if ((int)i != NEIGHBORS_FOCAL_FISH && !sim->fish[NEIGHBORS_FOCAL_FISH]->isNeighbor(i)){
                  spritesFish[i]->draw(&painter, vm, frameTmp);
          }
        }

        //Draw focal fish
        spritesFish[NEIGHBORS_FOCAL_FISH]->setColor(focusedColor);
        spritesFish[NEIGHBORS_FOCAL_FISH]->setTrajectoryColor(focusedColor);
        spritesFish[NEIGHBORS_FOCAL_FISH]->setDirectionColor(Qt::white);
        spritesFish[NEIGHBORS_FOCAL_FISH]->draw(&painter, vm, frameTmp);

        //Draw neighbors
        Fish* f = sim->fish[NEIGHBORS_FOCAL_FISH];
        for (unsigned int i=0; i<f->neighbors.size(); i++){
            if (f->neighbors[i] > -1 && f->neighbors[i]<NB_FISH) {
                spritesFish[f->neighbors[i]]->setColor(neighborsColors[i]);
                spritesFish[f->neighbors[i]]->setTrajectoryColor(neighborsColors[i]);
                spritesFish[f->neighbors[i]]->setDirectionColor(Qt::darkRed);
                spritesFish[f->neighbors[i]]->draw(&painter, vm, frameTmp);
            }
        }
    }
    else{
        for (unsigned int i = 0; i < spritesFish.size(); i++) {
          spritesFish[i]->draw(&painter, vm, frameTmp);
        }
    }

    ui->imglabel->setPixmap(pix);

    //if (writeimg && frameTmp == 0 && timer->isActive() && !sim->pause){
    if (dm->writeimg && frameTmp == 0 && !sim->pause){
        dm->writePicture(pix, imgfilecount);
        imgfilecount++;
    }

    frameCount++;
    frameTmp++;
  }

  stringstream sTmp;
  sTmp << sim->actualTime;
  sTmp << " s";
  //sTmp << " " << sim->fish[2]->speed;
  //sTmp << " " << ((Controller3D*)(sim->fish[2]->ctrl))->num_steps;
  //sTmp << " " << ((Controller3D*)(sim->fish[2]->ctrl))->num_kick;
  ui->timeLabel->setText(sTmp.str().c_str());
}

void MainWindow::processOneStep(){
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
            pause = true;

            //QApplication::quit();
            //exit(0);
        }

        sim->oneStep();

        dm->writeFishData();
        dm->computePDF();
        dm->computeTrajectories();

        nm->sender->writeDatagram();
        nm->toWriter->writeDatagram();

        //vm->computeCameraPosition();

        frameTmp = 0;

        //update();
    }
    else
    {
        if (sim->simulationFinished())
        {
          cout << "Finished !" << endl;
          cout << "# icpt = " << sim->nbIcpt << endl;
          cout << "Time of finish: " << sim->actualTime << endl;

          sim->pause = true;

          dm->savePDF();
          dm->saveTrajectories();
          //pause = true;

          //QApplication::quit();
          //exit(0);
        }
    }

    vm->computeCameraPosition();

    frameTmp = 0;
    update();
}

void MainWindow::onestep(){
    this->processOneStep();
}

void MainWindow::load()
{
    //Create sprites
    if (sprites.size() > 0){
        sprites.clear();
    }
    sprites = std::map<std::string, Sprite *>();

    if (spritesFish.size() > 0){
        spritesFish.clear();
    }
    spritesFish =std::vector<SpriteFish *>();

    if (sim != NULL){

        if (BACKGROUND){
            SpritePlainBackground* bg = new SpritePlainBackground();
            bg->dark_mode = DARK_BACKGROUND;
            this->addSprite("background", bg);
        }

        if (GRID){
            this->addSprite("grid", new SpriteGrid(GRID_SQUARE_SIZE));
        }

        if (sim->tank != NULL){
            this->addSprite("tank", new SpriteTank(sim->tank));
        }

        //Centers
//        SpriteCenter* spriteSchool = new SpriteCenter(sim);
//        spriteSchool->setColor(Qt::red);
//        spriteSchool->setDistributionCenter(false);
//        sprites.push_back(spriteSchool);

//        SpriteCenter* spriteDistrib = new SpriteCenter(sim);
//        spriteSchool->setColor(Qt::blue);
//        spriteSchool->setDistributionCenter(true);
//        sprites.push_back(spriteDistrib);

        if (!sim->fish.empty()){
            for (unsigned int i=0; i<sim->fish.size(); i++)
            {
                SpriteFish* sf = new SpriteFish();
                sf->setFish(sim->fish[i]);
                if (WHITE_FISH && !SHOW_NEIGHBORS){
                    sf->setColor(Qt::white);
                    sf->setDirectionColor(Qt::darkRed);
                    sf->setTrajectoryColor(Qt::white);
                }
                else if (SHOW_NEIGHBORS){
                    sf->setColor(Qt::gray);
                    sf->setDirectionColor(Qt::darkRed);
                    sf->setTrajectoryColor(Qt::gray);
                }
                else{
                     sf->setColor(colors[i%colors.size()]);
                     sf->setDirectionColor(Qt::darkRed);
                     sf->setTrajectoryColor(colors[i%colors.size()]);
                }
                sf->setShowFish(SHOW_FISH);
                sf->setShowTrajectory(SHOW_TRAJECTORY);
                sf->setTrajectoryLength(TRAJECTORY_LENGTH);
                sf->setTrajectoryWidth(TRAJECTORY_WIDTH);
                spritesFish.push_back(sf);
            }

            if (SHOW_NEIGHBORS){
                    spritesFish[NEIGHBORS_FOCAL_FISH]->setShowNeighbors(true);
            }
         }
    }
}

/** *************** **
 *  PRIVATE METHODS  *
 ** *************** **/

void MainWindow::initBoxes(){
    ui->positionsCheckBox->setChecked(dm->writedata);
    ui->picturesCheckBox->setChecked(dm->writeimg);
}

void MainWindow::initSliders(){
    if (FISHTANK && sim->tank != NULL){
        std::map<std::string, Sprite *>::iterator it_tank;
        it_tank = sprites.find("tank");
        ui->TankRedSlider->setSliderPosition(((SpriteTank*)(it_tank->second))->color.red());
        ui->TankGreenSlider->setSliderPosition(((SpriteTank*)(it_tank->second))->color.green());
        ui->TankBlueSlider->setSliderPosition(((SpriteTank*)(it_tank->second))->color.blue());
        ui->tankRedSpinbox->setValue(((SpriteTank*)(it_tank->second))->color.red());
        ui->tankGreenSpinbox->setValue(((SpriteTank*)(it_tank->second))->color.green());
        ui->tankBlueSpinbox->setValue(((SpriteTank*)(it_tank->second))->color.blue());
    }

    if (BACKGROUND){
        std::map<std::string, Sprite *>::iterator it_bg;
        it_bg = sprites.find("background");
        ui->BackRedSlider->setSliderPosition(((SpritePlainBackground*)(it_bg->second))->color.red());
        ui->BackGreenSlider->setSliderPosition(((SpritePlainBackground*)(it_bg->second))->color.green());
        ui->BackBlueSlider->setSliderPosition(((SpritePlainBackground*)(it_bg->second))->color.blue());
        ui->bgRedSpinbox->setValue(((SpritePlainBackground*)(it_bg->second))->color.red());
        ui->bgGreenSpinbox->setValue(((SpritePlainBackground*)(it_bg->second))->color.green());
        ui->bgBlueSpinbox->setValue(((SpritePlainBackground*)(it_bg->second))->color.blue());
    }
}

void MainWindow::initCircularControls(){
    unsigned int i=0;
    bool flag = false;
    while(!flag && i<this->sim->fish.size()){
        flag = std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "circular") == 0;
        i++;
    }
    if (flag){
        i--;
        ui->chkReverse->setChecked(((CircularController*)(this->sim->fish[i]->ctrl))->reverse);
        if (((CircularController*)(this->sim->fish[i]->ctrl))->burstandcoast){
            ui->circularSpeedBox->setValue(circle_vmean);
            ui->circularSpeedSlider->setSliderPosition((int)(circle_vmean*10));
        }
        else{
            ui->circularSpeedBox->setValue(circle_v0);
            ui->circularSpeedSlider->setSliderPosition((int)(circle_v0*10));
        }
    }
    else{
        ui->chkReverse->setEnabled(false);
        ui->circularSpeedBox->setEnabled(false);
        ui->circularSpeedSlider->setEnabled(false);
    }
}

void MainWindow::initRosaceControls(){
    unsigned int i=0;
    bool flag = false;
    while(!flag && i<this->sim->fish.size()){
        flag = std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "rosace") == 0;
        i++;
    }
    if (flag){
        i--;
        ui->chkReverseRosace->setChecked(((RosaceController*)(this->sim->fish[i]->ctrl))->reverse);
        ui->rosaceSpeedBox->setValue(rosace_v0);
        ui->rosaceSpeedSlider->setSliderPosition((int)(rosace_v0*10));
    }
    else{
        ui->chkReverseRosace->setEnabled(false);
        ui->rosaceSpeedBox->setEnabled(false);
        ui->rosaceSpeedSlider->setEnabled(false);
    }
}

void MainWindow::init3DModelControls(){
    unsigned int i=0;
    bool flag = false;
    while(!flag && i<this->sim->fish.size()){
        flag = std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "3d") == 0;
        i++;
    }

    if (true){
        ui->sb_c_v_0->setValue(C_v[0]);
        ui->sb_c_f_z_0->setValue(C_f_z[0]);
        ui->sb_c_f_att_d_1->setValue(C_f_att_d[1]);
        ui->sb_coef_adapt->setValue(Coef_Adapt);
        ui->sb_coef_adapt->setEnabled(false);
        ui->sb_coef_friction->setValue(Coef_Friction);
        ui->sb_coef_friction_z->setValue(Coef_Friction_z);
        ui->sb_coef_f_w->setValue(Coef_f_w);
        ui->sb_coef_f_rot->setValue(Coef_f_rot);
        ui->sb_coef_f_z->setValue(Coef_f_z);
        ui->sb_coef_f_att->setValue(Coef_f_att);
        ui->sb_coef_f_ali->setValue(Coef_f_ali);
        ui->sb_coef_f_att_z->setValue(Coef_f_att_z);
        ui->sb_f_c->setValue(f_c);
        ui->sb_d_c_att->setValue(d_c_att);
    }
    else{
        ui->vl3DModel->setEnabled(false);
    }
}

void MainWindow::resetAll()
{
    sim->pause = true;

    sim->reset();

    this->load();

    vm->reset();

    imgfilecount = 0;

    frameTmp = 1;
    frameTresh = 1;
    frameCount = 0;

    this->firstStep = true;
    ui->picturesCheckBox->setEnabled(true);
    ui->positionsCheckBox->setEnabled(true);
}


/** *********************** **
 *  IMPLEMENTATION OF SLOTS  *
 ** *********************** **/


void MainWindow::closeEvent(QCloseEvent * event)
{
    QApplication::quit();
}


void MainWindow::on_playButton_clicked()
{
  if (sim->simulationFinished()) {
    resetAll();
    this->setColors();
    update();
  }
  else {
    if (this->firstStep){
        if (dm->writeimg || dm->writedata){
            dm->createFoldersAndFiles();
        }
        this->firstStep = false;
        ui->picturesCheckBox->setEnabled(false);
        ui->positionsCheckBox->setEnabled(false);
    }
    sim->pause = ! sim->pause;
  }
}


void MainWindow::on_resetButton_clicked()
{
  this->resetAll();
  this->setColors();
  update();
}

void MainWindow::setColors(){

    // Tank colors
    std::map<std::string, Sprite *>::iterator it_tank;
    if (sim->tank != NULL){
        it_tank = sprites.find("tank");
        ((SpriteTank*)(it_tank->second))->color.setRed(ui->TankRedSlider->sliderPosition());
        ((SpriteTank*)(it_tank->second))->color.setGreen(ui->TankGreenSlider->sliderPosition());
        ((SpriteTank*)(it_tank->second))->color.setBlue(ui->TankBlueSlider->sliderPosition());

        ((SpriteTank*)(it_tank->second))->bgcolor.setRed(ui->BackRedSlider->sliderPosition());
        ((SpriteTank*)(it_tank->second))->bgcolor.setGreen(ui->BackGreenSlider->sliderPosition());
        ((SpriteTank*)(it_tank->second))->bgcolor.setBlue(ui->BackBlueSlider->sliderPosition());
    }

    //Background colors
    std::map<std::string, Sprite *>::iterator it_bg;
    if (BACKGROUND){
        it_bg = sprites.find("background");
        ((SpritePlainBackground*)(it_bg->second))->color.setRed(ui->BackRedSlider->sliderPosition());
        ((SpritePlainBackground*)(it_bg->second))->color.setGreen(ui->BackGreenSlider->sliderPosition());
        ((SpritePlainBackground*)(it_bg->second))->color.setBlue(ui->BackBlueSlider->sliderPosition());
    }
}

void MainWindow::on_exitButton_clicked()
{
  close();
  QApplication::quit();
}


void MainWindow::on_TankRedSlider_sliderMoved()
{
    int position = ui->TankRedSlider->sliderPosition();
    std::map<std::string, Sprite *>::iterator it_tank;
    if (sim->tank != NULL){
        it_tank = sprites.find("tank");
        ((SpriteTank*)(it_tank->second))->color.setRed(position);
    }

    ui->tankRedSpinbox->setValue(position);

    update();
}

void MainWindow::on_TankGreenSlider_sliderMoved()
{
    int position = ui->TankGreenSlider->sliderPosition();
    std::map<std::string, Sprite *>::iterator it_tank;
    if (sim->tank != NULL){
        it_tank = sprites.find("tank");
        ((SpriteTank*)(it_tank->second))->color.setGreen(position);
    }

    ui->tankGreenSpinbox->setValue(position);

    update();
}

void MainWindow::on_TankBlueSlider_sliderMoved()
{
    int position = ui->TankBlueSlider->sliderPosition();
    std::map<std::string, Sprite *>::iterator it_tank;
    if (sim->tank != NULL){
        it_tank = sprites.find("tank");
        ((SpriteTank*)(it_tank->second))->color.setBlue(position);
    }
    ui->tankBlueSpinbox->setValue(position);
    update();
}

void MainWindow::on_BackRedSlider_sliderMoved()
{
    int position = ui->BackRedSlider->sliderPosition();
    std::map<std::string, Sprite *>::iterator it_bg;

    if (sim->tank != NULL){
        it_bg = sprites.find("tank");
        ((SpriteTank*)(it_bg->second))->bgcolor.setRed(position);
    }

    if (BACKGROUND){
        it_bg = sprites.find("background");
        ((SpritePlainBackground*)(it_bg->second))->color.setRed(position);
    }

    ui->bgRedSpinbox->setValue(position);
    update();
}


void MainWindow::on_BackGreenSlider_sliderMoved()
{
    int position = ui->BackGreenSlider->sliderPosition();
    std::map<std::string, Sprite *>::iterator it_bg;

    if (sim->tank != NULL){
        it_bg = sprites.find("tank");
        ((SpriteTank*)(it_bg->second))->bgcolor.setGreen(position);
    }

    if (BACKGROUND){
        it_bg = sprites.find("background");
        ((SpritePlainBackground*)(it_bg->second))->color.setGreen(position);
    }
    ui->bgGreenSpinbox->setValue(position);
    update();
}


void MainWindow::on_BackBlueSlider_sliderMoved()
{
    int position = ui->BackBlueSlider->sliderPosition();
    std::map<std::string, Sprite *>::iterator it_bg;

    if (sim->tank != NULL){
        it_bg = sprites.find("tank");
        ((SpriteTank*)(it_bg->second))->bgcolor.setBlue(position);
    }

    if (BACKGROUND){
        it_bg = sprites.find("background");
        ((SpritePlainBackground*)(it_bg->second))->color.setBlue(position);
    }

    ui->bgBlueSpinbox->setValue(position);

    update();
}


void MainWindow::on_tankRedSpinbox_valueChanged()
{
    std::map<std::string, Sprite *>::iterator it_tank;
    if (sim->tank != NULL){
        it_tank = sprites.find("tank");
        ((SpriteTank*)(it_tank->second))->color.setRed(ui->tankRedSpinbox->value());
    }
    ui->TankRedSlider->setSliderPosition(ui->tankRedSpinbox->value());
    update();
}


void MainWindow::on_tankGreenSpinbox_valueChanged()
{
    std::map<std::string, Sprite *>::iterator it_tank;
    if (sim->tank != NULL){
        it_tank = sprites.find("tank");
        ((SpriteTank*)(it_tank->second))->color.setGreen(ui->tankGreenSpinbox->value());
    }
    ui->TankGreenSlider->setSliderPosition(ui->tankGreenSpinbox->value());
    update();
}


void MainWindow::on_tankBlueSpinbox_valueChanged()
{
    std::map<std::string, Sprite *>::iterator it_tank;
    if (sim->tank != NULL){
        it_tank = sprites.find("tank");
        ((SpriteTank*)(it_tank->second))->color.setBlue(ui->tankBlueSpinbox->value());
    }
    ui->TankBlueSlider->setSliderPosition(ui->tankBlueSpinbox->value());
    update();
}


void MainWindow::on_bgRedSpinbox_valueChanged()
{
    std::map<std::string, Sprite *>::iterator it_bg;
    if (sim->tank != NULL){
        it_bg = sprites.find("tank");
        ((SpriteTank*)(it_bg->second))->bgcolor.setRed(ui->bgRedSpinbox->value());
    }

    if (BACKGROUND){
        it_bg = sprites.find("background");
        ((SpritePlainBackground*)(it_bg->second))->color.setRed(ui->bgRedSpinbox->value());
    }

    ui->BackRedSlider->setSliderPosition(ui->bgRedSpinbox->value());
    update();
}


void MainWindow::on_bgGreenSpinbox_valueChanged()
{
    std::map<std::string, Sprite *>::iterator it_bg;
    if (sim->tank != NULL){
        it_bg = sprites.find("tank");
        ((SpriteTank*)(it_bg->second))->bgcolor.setGreen(ui->bgGreenSpinbox->value());
    }

    if (BACKGROUND){
        it_bg = sprites.find("background");
        ((SpritePlainBackground*)(it_bg->second))->color.setGreen(ui->bgGreenSpinbox->value());
    }
    ui->BackGreenSlider->setSliderPosition(ui->bgGreenSpinbox->value());
    update();
}


void MainWindow::on_bgBlueSpinbox_valueChanged()
{
    std::map<std::string, Sprite *>::iterator it_bg;
    if (sim->tank != NULL){
        it_bg = sprites.find("tank");
        ((SpriteTank*)(it_bg->second))->bgcolor.setBlue(ui->bgBlueSpinbox->value());
    }

    if (BACKGROUND){
        it_bg = sprites.find("background");
        ((SpritePlainBackground*)(it_bg->second))->color.setBlue(ui->bgBlueSpinbox->value());
    }

    ui->BackBlueSlider->setSliderPosition(ui->bgBlueSpinbox->value());
    update();
}


void MainWindow::on_sb_c_v_0_valueChanged(){
    C_v[0] = ui->sb_c_v_0->value();
}

void MainWindow::on_sb_c_f_z_0_valueChanged(){
    C_f_z[0] = ui->sb_c_f_z_0->value();
}

void MainWindow::on_sb_c_f_att_d_1_valueChanged(){
    C_f_att_d[1] = ui->sb_c_f_att_d_1->value();
}

void MainWindow::on_sb_coef_adapt_valueChanged(){
    Coef_Adapt = ui->sb_coef_adapt->value();
}

void MainWindow::on_sb_coef_friction_valueChanged(){
    Coef_Friction = ui->sb_coef_friction->value();
}

void MainWindow::on_sb_coef_friction_z_valueChanged(){
    Coef_Friction_z = ui->sb_coef_friction_z->value();
}

void MainWindow::on_sb_coef_f_w_valueChanged(){
    Coef_f_w = ui->sb_coef_f_w->value();
}

void MainWindow::on_sb_coef_f_rot_valueChanged(){
    Coef_f_rot = ui->sb_coef_f_rot->value();
}

void MainWindow::on_sb_coef_f_z_valueChanged(){
    Coef_f_z = ui->sb_coef_f_z->value();
}

void MainWindow::on_sb_coef_f_att_valueChanged(){
    Coef_f_att = ui->sb_coef_f_att->value();
}

void MainWindow::on_sb_coef_f_ali_valueChanged(){
    Coef_f_ali = ui->sb_coef_f_ali->value();
}

void MainWindow::on_sb_coef_f_att_z_valueChanged(){
    Coef_f_att_z = ui->sb_coef_f_att_z->value();
}

void MainWindow::on_sb_f_c_valueChanged(){
    f_c = ui->sb_f_c->value();
}

void MainWindow::on_sb_d_c_att_valueChanged(){
    d_c_att = ui->sb_d_c_att->value();
    //cout<<d_c_att<<endl;
}

void MainWindow::on_picturesCheckBox_stateChanged()
{
    dm->writeimg = ui->picturesCheckBox->isChecked();
}


void MainWindow::on_positionsCheckBox_stateChanged()
{
    dm->writedata = ui->positionsCheckBox->isChecked();
}

void MainWindow::on_chkReverse_stateChanged()
{
    //Find correct fish
    for (unsigned int i=0; i<this->sim->fish.size(); i++){
        if (std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "circular") == 0){
            ((CircularController*)(this->sim->fish[i]->ctrl))->setReverse(ui->chkReverse->isChecked());
        }
    }
}

void MainWindow::on_circularSpeedSlider_valueChanged()
{
    double newSpeed = ui->circularSpeedSlider->value()/10.;
    ui->circularSpeedBox->setValue(newSpeed);
    //Find correct fish
    for (unsigned int i=0; i<this->sim->fish.size(); i++){
        if (std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "circular") == 0){
            ((CircularController*)(this->sim->fish[i]->ctrl))->setMeanSpeed(newSpeed);
        }
    }
}

void MainWindow::on_circularSpeedBox_valueChanged()
{
    double newSpeed = ui->circularSpeedBox->value();
    ui->circularSpeedSlider->setSliderPosition((int)(newSpeed*10));
    //Find correct fish
    for (unsigned int i=0; i<this->sim->fish.size(); i++){
        if (std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "circular") == 0){
            ((CircularController*)(this->sim->fish[i]->ctrl))->setMeanSpeed(newSpeed);
        }
    }
}

void MainWindow::on_rosaceSpeedSlider_valueChanged()
{
    double newSpeed = ui->rosaceSpeedSlider->value()/10.;
    ui->rosaceSpeedBox->setValue(newSpeed);
    //Find correct fish
    for (unsigned int i=0; i<this->sim->fish.size(); i++){
        if (std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "rosace") == 0){
            ((RosaceController*)(this->sim->fish[i]->ctrl))->setMeanSpeed(newSpeed);
        }
    }
}

void MainWindow::on_rosaceSpeedBox_valueChanged()
{
    double newSpeed = ui->rosaceSpeedBox->value();
    ui->rosaceSpeedSlider->setSliderPosition((int)(newSpeed*10));
    //Find correct fish
    for (unsigned int i=0; i<this->sim->fish.size(); i++){
        if (std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "rosace") == 0){
            ((RosaceController*)(this->sim->fish[i]->ctrl))->setMeanSpeed(newSpeed);
        }
    }
}

void MainWindow::on_chkReverseRosace_stateChanged()
{
    //Find correct fish
    for (unsigned int i=0; i<this->sim->fish.size(); i++){
        if (std::strcmp(this->sim->fish[i]->ctrl->type.c_str(), "rosace") == 0){
            ((RosaceController*)(this->sim->fish[i]->ctrl))->setReverse(ui->chkReverseRosace->isChecked());
        }
    }
}

