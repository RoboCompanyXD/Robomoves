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

using namespace std;

Lidar lidar;
OCVCam cam;

ControlRobot con;

/*
 * 
 */
int main(int argc, char** argv) {
    
    // Inicializar robot
    
    //if(init()){}
    
    std::cout << "Hello World!";
    return 0;
    


    return 0;
}

int init(){
    
    // Controlar Init state (si se inicia todo bien)

    // Instanciar Objetos globales

    //  Instanciar Camara
    //cam = OCVCam();
    
    //  Instanciar Lidar
    //lidar = Lidar();
    
    // Inctanciar Controlrobot y pasarle Camara Lidar
    
    //con = ControlRobot(lidar,cam);

    

    // Crear Threads

    //  Crear Thread Camara
    //  Crear Thread Lidar

    return 0;
}