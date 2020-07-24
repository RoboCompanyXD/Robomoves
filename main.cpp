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
    
    while(1){
        sleep(100);
    }
    
    CamThread.join();
    LidarThread.join();

    return 0;
}
