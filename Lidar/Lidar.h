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

private:

    YdLidarX4 lidar; // TODO: documentar variable


    /**
     * TODO: Documentar Metodo
     */
    void LidarThread();



};

#endif	/* LIDAR_H */

