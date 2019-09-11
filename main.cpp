/* 
 * File:   main.cpp
 * Author: Borja Gómez
 *
 * Created on 28 de febrero de 2019, 12:42
 */

#include <cstdlib>
#include <stdlib.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include "Navegador/ControlRobot.h"
#include <wiringPi.h> 

#include <sys/stat.h>
#include <time.h>

#include <cv.h> 
#include <highgui.h>
#include <cxcore.h>



using namespace std;
using namespace cv;

void follow_cam(int cam);

int x,y,area,frame_width,frame_height,foto=0;

IRobotConnection robot;
ControlRobot robotfisico;

PI_THREAD (thread1){
       follow_cam(0);
    } 

/*
 * 
 */
int main(int argc, char** argv) {

    
    printf("Main: creando threads\n");
    int r; 
    r = piThreadCreate(thread1); 
    if(r != 0)
 {
 printf("Error en la creación de un thread!\n");
 } 
    
    robotfisico.inicializacion();
   
    while (!robotfisico.condicionSalida()){
        robotfisico.leerSensores();
        robotfisico.logicaEstados(x,y,area,frame_width,frame_height);
        robotfisico.moverActuadores();
        //robotfisico.imprimirInfo();
    }

    robotfisico.finalizacion();
}


void follow_cam(int cam)
{
    
    cout << "OpenCV version: "<< CV_VERSION << endl;
        
    struct stat sb;
    if (!(stat("/tmp/robonitor", &sb) == 0 && S_ISDIR(sb.st_mode))){
        const int dir_err = system("mkdir -p /tmp/robonitor");
        if (dir_err == -1)
            {
                printf("Error creating directory!n");
                exit(-1);
            }
        }
        
    clock_t start_t, end_t;
    start_t = clock();
      
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(90);
        
    Scalar lower_yellow = Scalar(24, 135, 93, 0);
    Scalar upper_yellow= Scalar(42, 252, 255, 0);
    Scalar lower_green = Scalar(54, 145, 16, 0);
    Scalar upper_green= Scalar(100, 255, 128, 0);
	
    VideoCapture cap(cam);

    Mat frame,hsv_frame,mask;

    Moments mu;
    Point2f mc;
    frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    while(1){
    	cap >> frame;
    	cvtColor(frame, hsv_frame, CV_BGR2HSV);         //Cambio del espacio de color
    	inRange(hsv_frame,lower_green,upper_green,mask);    //Filtrado
        
        erode(mask,mask,getStructuringElement(MORPH_ELLIPSE, Size(2,2)));  //Erosionar
                
        mu = moments(mask);  // Calculo momentos y posicion
        if (mu.m00>30000){      
    	   mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
           x = int(mc.x);
           y = int(mc.y);
           area = int(mu.m00);
        }else{
            mc = Point2f(0,0);
            x=0;
            y=0;
            area=0;
        }
        end_t = clock();
        
        if ((double)(end_t - start_t) / CLOCKS_PER_SEC > 1.0){   // Guardar captura cada segundo
            start_t = end_t;
            system("touch /tmp/robonitor/.lock");
            imwrite("/tmp/robonitor/cap.jpg", frame, compression_params);
            system("unlink /tmp/robonitor/.lock");
        }

    }
}

//COMENTARIO DI MERDA es una 