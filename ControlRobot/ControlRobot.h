// Control Robot.h

#ifndef CONTROLROBOT_H
#define	CONTROLROBOT_H

//#pragma once

#include "../IRobotFramework/IRobotConnection.h"
#include "../Lidar/Lidar.h"
#include "../OCVCam/OCVCam.h"
#include <iostream>

//#include "../RobotStateMachine/RobotStateMachine.h"

class RobotStateMachine; // Forward declaration

/**
 * TODO: Documentar clase
 * - para que sirve?
 */
class ControlRobot {
public:


    ControlRobot();
    ControlRobot(Lidar l, OCVCam c);
    ~ControlRobot(void);

    Lidar lidar;
    OCVCam cam;

    RobotStateMachine * uc;

    void inicializacion();
    void finalizacion();
    bool condicionSalida();
    void leerSensores();
    void logicaEstados(int x, int y, int area, int frame_width, int frame_height);
    void moverActuadores();
    void imprimirInfo();


    void drive(int der, int izq);

    void setMotores_actual(char state);


    /**
     * TODO: Documentar metodo
     */
    void gotoDock();

    /**
     * TODO: Documentar metodo (mas detallado)
     * Calcular a donde ir con la camara
     */
    void computeCameraApproach();

    /**
     * TODO: Documentar metodo (mas detallado)
     * Calcular a donde ir con la camara
     */
    void computeCameraWithObstacle();


    /**
     * Play "blocked" sound
     */
    void reproducirSonidoBloqueado();

    /**
     * Play "un-blocked" sound
     */
    void reproducirSonidoDesbloqueado();


    /**
     * Calcular a donde ir con el lidar
     */
    void computeLidarTripPersonOutOfView();

    //// TODO: ¿por qué no poner los metodos anteriores dentro de la definición de la clase?

    bool check_btnSpot(); // TODO: documentar
    bool check_btnClean();
    bool check_btnDock();


private:

    IRobotConnection *robot;

    struct Sensores_iCreate {
        // variables para almacenar informaci�n del
        // sensor de acantilado frontal izquierdo
        unsigned int front_left;
        bool fl;                                               // Sensor Virtual

        unsigned int front_right;
        bool fr;                                               // Sensor Virtual

        unsigned int left;
        bool l;                                                // Sensor Virtual

        unsigned int right;
        bool r;                                                // Sensor Virtual


        unsigned int buttons;
        bool play;                                             // Sensor Virtual
        bool avance;                                           // Sensor Virtual

        unsigned int bumpers;
        bool bl;                                               // Sensor Virtual
        bool br;                                               // Sensor Virtual

        int angle;
        int distance;
        int sum_angle;                              // Sensor Virtual
        int sum_distance;                                      // Sensor Virtual

        unsigned int lightbumper;
        unsigned int lbl;                                      // Sensor Virtual
        unsigned int lbfl;                                     // Sensor Virtual
        unsigned int lbcl;                                     // Sensor Virtual
        unsigned int lbcr;                                     // Sensor Virtual
        unsigned int lbfr;                                     // Sensor Virtual
        unsigned int lbr;                                      // Sensor Virtual
        
        bool lbump_front;
        bool lbump_side;
        
        bool clean; //LED CLEAN

        int charger_available;
        bool IsDocked;                                         // Sensor Virtual

        int battery_charge;
        int battery_capacity;
        int battery_percentage;                                // Sensor Virtual
        
        // Sensores Cliff
        bool cliff_left;
        bool cliff_frontleft;
        bool cliff_frontright;
        bool cliff_right;
        bool cliff;                                             // Sensor Virtual
        
        
    }; // struct sensores_iCreate

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

};

#endif // CONTROLROBOT_H