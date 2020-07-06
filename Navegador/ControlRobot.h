// Control Robot.h

#ifndef CONTROLROBOT_H
#define	CONTROLROBOT_H

//#pragma once

#include "../IRobotFramework/IRobotConnection.h"
#include "../Lidar/Lidar.h"
#include "../OCVCam/OCVCam.h"
#include <iostream>

// #include "../UC/UC.h"

class UC; // Forward declaration

/**
 * TODO: Documentar clase
 * - para que sirve?
 */
class ControlRobot {

private:

    IRobotConnection *robot;
    

    struct Sensores_iCreate {

        // variables para almacenar informaci�n del
        // sensor de acantilado frontal izquierdo
        unsigned int front_left;
        bool fl;

        unsigned int front_right;
        bool fr;

        unsigned int left;
        bool l;

        unsigned int right;
        bool r;


        unsigned int buttons;
        bool play;
        bool avance;

        unsigned int bumpers;
        bool bl;
        bool br;

        int angle;
        int distance;
        int sum_angle, sub_angle;
        int sum_distance;

        int chargstate;
        unsigned int lightbumper;
        unsigned int lbl;
        unsigned int lbfl;
        unsigned int lbcl;
        unsigned int lbcr;
        unsigned int lbfr;
        unsigned int lbr;

        bool clean; //LED CLEAN

        int charger_available;
        
        int battery_charge;
        int battery_capacity;
        int battery_percentage;
    };
    

    struct Actuadores_iCreate {

        // variables para el comando Drive direct
        int vel_der;
        int vel_izq;
        // variables para el comando Drive
        int velocidad;
        int giro;
    };
    struct Actuadores_iCreate actuadores;

    char estado_actual;
    char estado_anterior;
    char motores_actual;
    char motores_anterior;

public:
    
    struct Sensores_iCreate sensores;
    ControlRobot();
    ControlRobot(Lidar l, OCVCam c);
    ~ControlRobot(void);
    
    Lidar lidar;
    OCVCam cam;

    UC * uc;

    void inicializacion();
    void finalizacion();
    bool condicionSalida();
    void leerSensores();
    void logicaEstados(int x, int y, int area, int frame_width, int frame_height);
    void moverActuadores();
    void imprimirInfo();
    void computeCamaraApproach();
    void computeCamaraWithObstacle();
    void gotoDock();

    void drive(int der, int izq);
    
    
    
    void setMotores_actual(char state);


};

#endif // CONTROLROBOT_H