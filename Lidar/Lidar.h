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

#define NUM_SECTORS 8      // Numero de sectores en los que se dividirán los 360 grados
#define OBSTACLEMINDISTANCE 50

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
    void LidarThread();
    void exitLidarThread();

private:

    YdLidarX4::YdLidarX4Controller myLidar; // TODO: documentar variable

    /**
     * TODO: Documentar Metodo
     */


    int anglestep;
    int * sectorstartangles; // Array con los indices de donde empieza cada sector
    int * sectorendangles; // Array con los indices de donde acaba cada sector

    int lidarstate;

    enum LidarStates {
        Idle = 0,
        GoToIdle = 1,
        Scanning = 2,
        GoToScanning = 3

    };

    int * lastSample;
    
    int indexofSmallestElement(int * arr, int size);
    
    bool runLidarThread;


};

#endif	/* LIDAR_H */

