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

// Variables globales de la camara, el lidar y el robot
Lidar * lidar;
OCVCam * cam;
ControlRobot::ControlRobot * mRobotController;

// Variables globales de los threads
thread LidarServerThread;
thread LidarThread;
thread CamThread;

void CameraThreadFunc(OCVCam * camptr) {

    camptr->AnalyzeCam();
}

void LidarThreadFunc(Lidar * lidarptr) {

    lidarptr->LidarThread();
}

void exit_end() {
    lidar->exitLidarThread();
    cam->exitCamThread();

    sleep(1);
}

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

    // Instanciar Objetos globales
    cam = new OCVCam();
    lidar = new Lidar();

    // Inctanciar Controlrobot y pasarle Camara Lidar
    mRobotController = new ControlRobot::ControlRobot(lidar, cam);

    // Crear Threads Camara y Lidar
    CamThread = thread(&CameraThreadFunc,cam);
    LidarThread = thread(&LidarThreadFunc, lidar);
    lidar->setLidarScanning(); // TODO CRITICAL: is this OK?
    
    while(1){
        sleep(100);
    }
    
    CamThread.join();
    LidarThread.join();

    return 0;
}
