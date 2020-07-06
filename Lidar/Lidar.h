/**
 * File:   Lidar.h
 * Author: chris
 *
 * Created on 19 de junio de 2020, 13:05
 */

#ifndef LIDAR_H
#define	LIDAR_H

#include "YdLidarX4/YdLidarX4.h"

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

    int computedAngle; // TODO: documentar
    int computedDistance; // TODO: documentar

private:

    YdLidarX4::YdLidarX4Controller myLidar; // TODO: documentar variable

    /**
     * TODO: Documentar Metodo
     */
    void LidarThread();

};

#endif	/* LIDAR_H */

