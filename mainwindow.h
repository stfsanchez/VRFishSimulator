/*****************************************************************************************
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: © 2022 Stephane Sanchez <stephane.sanchez@ut-capitole.fr>
 * Université Toulouse Capitole - Institut de Recherche en Informatique de Toulouse
 * Real Expression Artificial Life (REVA) Research Team
 * https://www.irit.fr/departement/calcul-intensif-simulation-optimisation/reva/
 *****************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <vector>
#include <map>

#include <ctime>

#include <QtWidgets/QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QTimer>

#include "Core/simulator.h"
#include "Graphics/sprite.h"
#include "Graphics/spritefish.h"
#include "Graphics/viewmodifier.h"
#include "Tools/datamanager.h"
#include "Tools/networkmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        std::map<std::string, Sprite *> sprites;
        std::vector<SpriteFish *> spritesFish;
        ViewModifier * vm;

        Simulator * sim;

        DataManager * dm;
        NetworkManager * nm;

        bool firstStep;
        bool pause;

        unsigned int imgfilecount = 0;

        /* Window geometry */
        int window_width;
        int window_height;

        /* Scaling factor - conversion from meter to pixel */
        int world_to_pixel;

        /* For display changes */
        int frameTmp;
        int frameCount;
        int frameTresh;

        vector<QColor> colors;
        QColor focusedColor;
        QColor standardColor;
        vector<QColor> neighborsColors;

        explicit MainWindow(int width, int height, int scaling, QWidget *parent = 0, Simulator* nsim = 0, DataManager * dm = 0, NetworkManager * nm = 0);

        ~MainWindow();

        inline void setSimulator(Simulator* nsim) { sim=nsim; }
        inline void addSprite(std::string name, Sprite * s) { sprites.insert({name, s}); }
        inline void addSpriteFish(SpriteFish * f) { spritesFish.push_back(f); }
        void load();
        void setWriteFlags(bool writedata, bool writeimg);

        void processOneStep();

    public slots:
        void onestep();

    private:

        Ui::MainWindow *ui;
        QTimer *timer;

        void initBoxes();
        void initSliders();
        void initCircularControls();
        void initRosaceControls();
        void init3DModelControls();
        void resetAll();
        void createFoldersAndFiles();

    private slots:
        void paintEvent(QPaintEvent * event);

        void setColors();

        void on_playButton_clicked();
        void on_resetButton_clicked();
        void on_exitButton_clicked();

        void closeEvent(QCloseEvent * event);

        void on_TankRedSlider_sliderMoved();
        void on_BackRedSlider_sliderMoved();
        void on_BackGreenSlider_sliderMoved();
        void on_BackBlueSlider_sliderMoved();
        void on_TankGreenSlider_sliderMoved();
        void on_TankBlueSlider_sliderMoved();
        void on_tankRedSpinbox_valueChanged();
        void on_tankGreenSpinbox_valueChanged();
        void on_tankBlueSpinbox_valueChanged();
        void on_bgRedSpinbox_valueChanged();
        void on_bgGreenSpinbox_valueChanged();
        void on_bgBlueSpinbox_valueChanged();
        void on_picturesCheckBox_stateChanged();
        void on_positionsCheckBox_stateChanged();
        void on_chkReverse_stateChanged();
        void on_circularSpeedSlider_valueChanged();
        void on_circularSpeedBox_valueChanged();
        void on_rosaceSpeedSlider_valueChanged();
        void on_rosaceSpeedBox_valueChanged();
        void on_chkReverseRosace_stateChanged();

        void on_sb_c_v_0_valueChanged();
        void on_sb_c_f_z_0_valueChanged();
        void on_sb_c_f_att_d_1_valueChanged();
        void on_sb_coef_adapt_valueChanged();
        void on_sb_coef_friction_valueChanged();
        void on_sb_coef_friction_z_valueChanged();
        void on_sb_coef_f_w_valueChanged();
        void on_sb_coef_f_rot_valueChanged();
        void on_sb_coef_f_z_valueChanged();
        void on_sb_coef_f_att_valueChanged();
        void on_sb_coef_f_ali_valueChanged();
        void on_sb_coef_f_att_z_valueChanged();
        void on_sb_f_c_valueChanged();
        void on_sb_d_c_att_valueChanged();
};

#endif // MAINWINDOW_H
