/**
 * File:   Lidar.h
 * Author: chris
 *
 * Created on 19 de junio de 2020, 13:05
 */

#ifndef LIDAR_H
#define	LIDAR_H

#include "YdLidarX4/YdLidarX4.h"
#include <unistd.h>

/**
 * TODO: Documentar Clase
 * 
 * TODO: ¿Por qué no unir este fichero/clase con YdLidarX4???
 */
class Lidar {

public:

    /**
     * Class Constructor
     */
    Lidar();

    /**
     * Class desctructor
     */
    virtual ~Lidar();

    //set_lidar_params();

    // TODO: ¿Por qué no hacer estas variables private?
    bool isObstable; // TODO: documentar
    
    bool isFrontLibre;
    bool isBackLibre;

    int computedAngle; // TODO: documentar
    int computedDistance; // TODO: documentar
    
    void setLidarScanning();
    void setLidarIdle();
    
    void computeLidarTripPersonOutOfView();

private:

    YdLidarX4::YdLidarX4Controller myLidar; // TODO: documentar variable

    /**
     * TODO: Documentar Metodo
     */
    void LidarThread();
    
    int * sectorstartangles; // Array con los indices de donde empieza cada sector
    int * sectorendangles; // Array con los indices de donde acaba cada sector
    
    int lidarstate;
    
    enum LidarStates{
    
        Idle = 0,
        GoToIdle = 1,
        Scanning = 2,
        GoToScanning = 3
        
    };
    
    int * lastSample;
    

};

#endif	/* LIDAR_H */

