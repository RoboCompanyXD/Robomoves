/**
 * File:   main.h
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#ifndef MAIN_H
#define	MAIN_H

#include <cstdlib>

#include "Lidar/Lidar.h"
#include "OCVCam/OCVCam.h"
#include "Navegador/ControlRobot.h"

/**
 * TODO: Documentar Main
 */

int init();

Lidar main_lidar;
OCVCam main_cam;

ControlRobot main_con;



//int main(int argc, char** argv);


#endif	/* MAIN_H */

