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
    cam = OCVCam();
    
    //  Instanciar Lidar
    lidar = Lidar();
    
    // Inctanciar Controlrobot y pasarle Camara Lidar
    
    con = ControlRobot(lidar,cam);

    

    // Crear Threads

    //  Crear Thread Camara
    //  Crear Thread Lidar

    return 0;
}