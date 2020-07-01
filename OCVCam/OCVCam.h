/*  
 * File:   OCVCam.h 
 * Author: Borja Gómez 
 * 
 * Created on 19 de junio de 2020, 13:05 
 */ 
 
#ifndef OCVCAM_H 
#define	OCVCAM_H 
 
#include <cstdlib> 
#include <stdlib.h> 
#include <poll.h> 
#include <termios.h> 
#include <unistd.h> 
#include <iostream> 
//#include "ControlRobot.h" 
#include <wiringPi.h> 
#include <vector> 
 
#include <sys/stat.h> 
#include <time.h> 
 
//#include <cv.h> 
#include <opencv2/highgui.hpp> 
#include <opencv2/core.hpp> 
 
#include <opencv2/videoio.hpp> 
#include <opencv2/imgcodecs.hpp> 
#include <opencv2/imgproc.hpp> 
#include <opencv2/objdetect.hpp> 
#include <opencv2/highgui.hpp> 
 
 
class OCVCam{ 
public: 
    OCVCam(void); 
    virtual ~OCVCam(void); 
     
    int AnalyzeCam(void);  
     
    int x; 
    int y; 
    int area; 
    int frame_width; 
    int frame_height; 
     
}; 
 
#endif	/* OCVCAM_H */ 
