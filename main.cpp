/**
 * File:   main.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#include <cstdlib>

#include "Lidar/Lidar.h"
#include "OCVCam/OCVCam.h"
#include "ControlRobot/ControlRobot.h"

#include <iostream>
#include <thread> 
#include <stdio.h>
#include <python3.7/Python.h> 

// Variable globale del robot

ControlRobot::ControlRobot * mRobotController;

// Variable globale del thread de lidarserver
thread LidarServerThread;

// TODO: Probar esta o buscar una manera de lanzar el codigo del servidor
void LidarServerThreadFunc() {
    char filename[] = "YdLidarX4PythonInterface/lidarator.py";
    FILE* fp;

    Py_Initialize();

    fp = _Py_fopen(filename, "r");
    PyRun_SimpleFile(fp, filename);

    Py_Finalize();
}

void startLidarServer() {
    // Crear thread y ejecutar programa de python
    LidarServerThread = thread(LidarServerThreadFunc);
}

int main(int argc, char** argv) {
    std::cout << "Iniciando..." << std::endl;

    //startLidarServer(); // Lanzar servidor del YlidarX4

    // Inctanciar Controlrobot y pasarle Camara Lidar
    mRobotController = new ControlRobot::ControlRobot();
    
    while(!mRobotController.condicionSalida()){
        
        mRobotController.doBehavior();
        
        
        //sleep(100);
    }
    
    mRobotController.endBehavior();


    return 0;
}
