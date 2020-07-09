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


using namespace std;

// Variables globales de la camara, el lidar y el robot
    Lidar lidar;
    OCVCam cam;
    ControlRobot::ControlRobot mRobotController;

// Variables globales de los threads
    
    thread LidarServerThread;
    thread LidarTread;
    thread CamThread;

    int init();
    void startLidarServer();

    
/*
 * 
 */
int main(int argc, char** argv) {
    
    // Lanzar servidor del YlidarX4
    
    startLidarServer();
    
    // Inicializar robot
    
    std::cout << "Iniciando..." << std::endl;
    
    if(init()){}
    
    std::cout << "Hello World!";
    return 0;
    


    return 0;
}

void CameraThreadFunc(OCVCam * camptr){

    camptr->AnalyzeCam();
}

void LidarThreadFunc(Lidar * lidarptr){

    lidarptr->LidarThread();
}

/**
 * TODO: documentar
 */
int init(){
    
    // Controlar Init state (si se inicia todo bien)

    // Instanciar Objetos globales

    //  Instanciar Camara
    cam = OCVCam();
    
    //  Instanciar Lidar
    lidar = Lidar();
    
    // Inctanciar Controlrobot y pasarle Camara Lidar
    
    mRobotController = ControlRobot::ControlRobot(lidar,cam);

    

    // Crear Threads

    //  Crear Thread Camara
    
    CamThread = thread(&CameraThreadFunc,&cam);
    
    //  Crear Thread Lidar
    
    LidarTread = thread(&LidarThreadFunc,&lidar);
    

    return 0;
}

void exit_end(){
    
    lidar.exitLidarThread();
    cam.exitCamThread();
    
    sleep(1);
    
    
}

void LidarServerThreadFunc(){
    
    char filename[] = "YdLidarX4PythonInterface/lidarator.py"; 
	FILE* fp; 
 
	Py_Initialize(); 
 
	fp = _Py_fopen(filename, "r"); 
	PyRun_SimpleFile(fp, filename); 
 
	Py_Finalize(); 

}

void startLidarServer(){
    
    // Crear thread y ejecutar programa de python
    
    //LidarServerThread = thread(LidarServerThreadFunc);
}

