/**
 * File:   main.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#include "main.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    // Inicializar robot
    
    if(init()){}
    


    return 0;
}

int init(){
    
    // Controlar Init state (si se inicia todo bien)

    // Instanciar Objetos globales

    //  Instanciar Camara
    main_cam = OCVCam();
    
    //  Instanciar Lidar
    main_lidar = Lidar();
    
    // Inctanciar Controlrobot y pasarle Camara Lidar
    
    main_con = ControlRobot(main_lidar, main_cam);

    

    // Crear Threads

    //  Crear Thread Camara
    //  Crear Thread Lidar

    return 0;
}