/**
 * File:   ControlRobot.h
 * Author: chris
 *
 * Created on 10 de enero de 2020, 12:33
 */

#ifndef CONTROLROBOT_H
#define	CONTROLROBOT_H

//#pragma once

#include "../IRobotFramework/IRobotConnection.h"
#include "../Lidar/Lidar.h"
#include "../OCVCam/OCVCam.h"
#include <iostream>

//#include "../RobotStateMachine/RobotStateMachine.h"

class RobotStateMachine; // Forward declaration

namespace ControlRobot {

    /**
     * Class ControlRobot
     * 
     * This class serves as an interface to control the real/physical robot. 
     * In a sense, it is like a robot controller interface.
     * 
     * You'll need an instance of this class in order to command the robot.
     * 
     */
    class ControlRobot {

    public:

        /**
         * Class ControlRobot default constructor
         */
        ControlRobot();

        /**
         * Class ControlRobot constructor
         * 
         * @param l Lidar instance
         * @param c Camera instance
         */
        ControlRobot(Lidar l, OCVCam c);

        /**
         * Class ControlRobot destructor
         */
        ~ControlRobot(void);

        // TODO: ¿Por qué no hacer private estas variables?
        Lidar lidar; // TODO documentar
        OCVCam cam; // TODO documentar

        RobotStateMachine * stateMachine; // TODO: refactor to Smart Pointer

        /**
         * Initialize controller and stablish connection to the real robot device.
         */
        void inicializacion();

        /**
         * Terminate connection to the real robot device.
         */
        void finalizacion();

        /**
         * TODO: documentar
         * 
         * @return 
         */
        bool condicionSalida();

        /**
         * TODO: documentar
         */
        void leerSensores();

        /**
         * TODO: documentar
         * 
         * @param x
         * @param y
         * @param area
         * @param frame_width
         * @param frame_height
         */

        /**
         * 
         */
        void logicaEstados(int x, int y, int area, int frame_width, int frame_height);

        /**
         * TODO: documentar
         */
        void moverActuadores();

        /**
         * TODO: documentar
         */
        void imprimirInfo();

        /**
         * TODO: documentar
         * 
         * @param der
         * @param izq
         */
        void drive(int der, int izq);

        /**
         * TODO: documentar
         * 
         * @param state
         */
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

        /**
         * TODO: documentar
         * 
         * @return 
         */
        bool check_btnSpot();

        /**
         * TODO: documentar
         * 
         * @return 
         */
        bool check_btnClean();

        /**
         * TODO: documentar
         * 
         * @return 
         */
        bool check_btnDock();


    private:
        
        // TODO: refactor to Smart Pointer
        IRobotConnection * robot; // TODO: documentar

        /**
         * TODO: documentar
         */
        struct Sensores_iCreate {

            // variables para almacenar información del
            // sensor de acantilado frontal izquierdo
            unsigned int front_left;
            bool fl; // Sensor Virtual

            unsigned int front_right;
            bool fr; // Sensor Virtual

            unsigned int left;
            bool l; // Sensor Virtual

            unsigned int right;
            bool r; // Sensor Virtual


            unsigned int buttons;
            bool button_dock; // Sensor Virtual
            bool button_clean; // Sensor Virtual
            bool button_spot; // Sensor Virtual
            bool button_day; // Sensor Virtual
            bool button_hour; // Sensor Virtual
            bool button_minute; // Sensor Virtual
            bool button_schedule; // Sensor Virtual
            bool button_clock; // Sensor Virtual
            
            unsigned int bumpers;
            bool bl; // Sensor Virtual
            bool br; // Sensor Virtual

            int angle;
            int distance;
            int sum_angle; // Sensor Virtual
            int sum_distance; // Sensor Virtual

            unsigned int lightbumper;
            unsigned int lbl; // Sensor Virtual
            unsigned int lbfl; // Sensor Virtual
            unsigned int lbcl; // Sensor Virtual
            unsigned int lbcr; // Sensor Virtual
            unsigned int lbfr; // Sensor Virtual
            unsigned int lbr; // Sensor Virtual

            bool lbump_front; // Sensor Virtual
            bool lbump_side; // Sensor Virtual

            bool led_clean; //LED CLEAN

            int charger_available;
            bool IsDocked; // Sensor Virtual

            int battery_charge;
            int battery_capacity;
            int battery_level; // Sensor Virtual

            // Sensores Cliff
            bool cliff_left;
            bool cliff_frontleft;
            bool cliff_frontright;
            bool cliff_right;
            bool cliff; // Sensor Virtual


        }; // struct Sensores_iCreate

        struct Actuadores_iCreate {

            int vel_der; // variable de velocidad para el comando Drive direct
            int vel_izq; // variable de velocidad para el comando Drive direct

            int velocidad; // variable de velocidad para el comando Drive
            int giro; // variable de velocidad para el comando Drive
        }; // struct Actuadores_iCreate

        Actuadores_iCreate actuadores;

        char estado_actual; // TODO: estas variables no tendrían que estar aquí, sino en la clase RobotStateMachine
        char estado_anterior;
        char motores_actual;
        char motores_anterior;
        
        bool dock_anterior;
        bool clean_anterior;
        bool spot_anterior;

    public:
        Sensores_iCreate sensores;

    };

} //namespace ControlRobot

#endif // CONTROLROBOT_H