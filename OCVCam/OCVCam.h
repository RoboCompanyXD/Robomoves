/**  
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

/**
 * TODO: Documentar clase
 */
class OCVCam {

public:

    /**
     * TODO: Documentar clase
     */
    OCVCam();

    /**
     * TODO: Documentar clase
     */
    virtual ~OCVCam();

    /**
     * TODO: Documentar clase
     */
    int AnalyzeCam();

    int x; // TODO: Documentar
    int y; // TODO: Documentar
    int area; // TODO: Documentar
    int frame_width; // TODO: Documentar
    int frame_height; // TODO: Documentar
    bool isPersonInView; // TODO: Documentar

};

#endif	/* OCVCAM_H */ 
