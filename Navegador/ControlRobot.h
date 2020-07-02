// Control Robot.h
#pragma once

#include "../IRobotFramework/IRobotConnection.h"
#include "../Lidar/Lidar.h"
#include "../OCVCam/OCVCam.h"
#include <iostream>


/**
 * TODO: Documentar clase
 * - para que sirve?
 */
class ControlRobot {
private:

    IRobotConnection *robot;
    Lidar lidar;
    OCVCam cam;

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
    };
    struct Sensores_iCreate sensores;

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
    ControlRobot(Lidar l, OCVCam c);
    ~ControlRobot(void);

    void inicializacion();
    void finalizacion();
    bool condicionSalida();
    void leerSensores();
    void logicaEstados(int x, int y, int area, int frame_width, int frame_height);
    void moverActuadores();
    void imprimirInfo();
    void computeCamaraApproach();
    void computeCamaraWithObstacle();

    void drive(int der, int izq);
};

