/**
 * File:   ControlRobot.cpp
 * Author:
 *
 * Created on 10 de enero de 2020, 12:33
 */

#include "ControlRobot.h"
#include <iostream>

#include<bitset>

//#define BAUD 57600
#define BAUD 115200

using namespace std;

#define VELGIR 80
#define VELCERO 0
#define VELAV 150

#define READY 'a'
#define WAIT 'b'
#define MOVCAM 'c'
#define RET 'd'
#define GIR 'e'
#define AV 'f'
#define AVPLUS 'g'
#define GIRPLUS 'h'
#define GIR2 'i'

#define STOP 'r'
#define FWD 's'
#define BACK 't'
#define LEFT 'u'
#define RIGHT 'v'
#define FWD_L 'w'
#define FWD_R 'x'
#define BACK_L 'y'
#define BACK_R 'z'

ControlRobot::ControlRobot(){
}

/**
 * ControlRobot class constructor
 */
ControlRobot::ControlRobot(Lidar l, OCVCam c) {
    this->lidar = l;
    this->cam = c;
       
    // Instanciar maquina de estados
    
    //this->uc = UC_ControlRobot();

}

/**
 * ControlRobot class destructor
 */
ControlRobot::~ControlRobot(void) {

}

/**
 * TODO: documentar
 */
void ControlRobot::inicializacion(void) {

    int COM_port;
    char puerto[30];

    // Solicitamos el puerto COM por entrada estandar
    //cout << "Puerto COM: ";
    //cin >> COM_port;
    //sprintf(puerto, "COM%d",COM_port);

    sprintf(puerto, "/dev/ttyUSB0");

    robot = new IRobotConnection(puerto);

    //robot->setVerboseMode(true);

    // Iniciamos la conexion:
    cout << "Connecting... ";
    //robot->connect();
    robot->connect(BAUD);
    cout << "Done!!\n" << endl;

    // Comando 128 start
    robot->start();
    delay(500); // Esperamos medio segundo a que cambie de modo

    // Comando 132 modo full
    robot->full();
    delay(500); // Esperamos medio segundo a que cambie de modo


    estado_actual = READY;
    estado_anterior = READY;
    motores_actual = STOP;
    motores_anterior = STOP;
}

/**
 * TODO: documentar metodo
 */
void ControlRobot::finalizacion(void) {
    robot->stop();
}


int nsensores = 11; // TODO: documentar variable

/** TODO: Documentar variable*/
char lsensores[] = {
    iRobotSensors::BUTTONS,
    iRobotSensors::BUMPERS_AND_WHEELDROPS,
    iRobotSensors::LIGHTBUMPER,
    iRobotSensors::LIGHTBUMPLEFT,
    iRobotSensors::LIGHTBUMPFRONTLEFT,
    iRobotSensors::LIGHTBUMPCENTERLEFT,
    iRobotSensors::LIGHTBUMPCENTERRIGHT,
    iRobotSensors::LIGHTBUMPFRONTRIGHT,
    iRobotSensors::LIGHTBUMPRIGHT,
    iRobotSensors::DISTANCE,
    iRobotSensors::ANGLE
};

/** TODO: Documentar variable*/char lsensores2[] = {
    iRobotSensors::BUTTONS,
    iRobotSensors::BUMPERS_AND_WHEELDROPS,
};

int *data; // TODO: documentar variable
int *dataserie; // TODO: documentar variable

/**
 * TODO: documentar metodo
 */
void ControlRobot::leerSensores() {

    data = robot->queryList(lsensores, nsensores);

    sensores.buttons = data[0];
    sensores.bumpers = data[1];
    sensores.lightbumper = data[2];
    sensores.lbl = data[3];
    sensores.lbfl = data[4];
    sensores.lbcl = data[5];
    sensores.lbcr = data[6];
    sensores.lbfr = data[7];
    sensores.lbr = data[8];

    if (data[9] > 32767)sensores.distance = data[9] - 65536;
    else sensores.distance = data[9];

    if (data[10] > 32767)sensores.angle = data[10] - 65536;
    else sensores.angle = data[10];

    sensores.br = sensores.bumpers & 0b00000001;
    sensores.bl = sensores.bumpers & 0b00000010;

    sensores.sum_distance += sensores.distance;
    sensores.sum_angle += sensores.angle;
}

/**
 * TODO: documentar metodo
 */
void ControlRobot::imprimirInfo(void) {
    if (estado_anterior != estado_actual) {
        cout << "ESTADO: " << estado_actual << "         MOTORES: " << motores_actual << endl;
    }
}

/**
 * TODO: documentar
 * - Detallar un poco cada estado
 * - Indicar donde se encuentra la documentación completa (grafo de estados etc)
 */
void ControlRobot::logicaEstados(int x, int y, int area, int frame_width, int frame_height) {

    //uc.statechart_process();

    estado_anterior = estado_actual;
    switch (estado_anterior) {
        case READY:
            motores_actual = STOP;
            if (sensores.buttons == 0b00000001)estado_actual = WAIT;
            break;
        case WAIT:
            motores_actual = STOP;
            if (x != 0 && y != 0)estado_actual = MOVCAM;
            break;
        case MOVCAM:
            //motores_actual=STOP;
            if (sensores.br == 1 || sensores.bl == 1) {
                estado_actual = RET;
                sensores.sum_distance = 0;
                sensores.sum_angle = 0;
            } else if (x == 0 || y == 0)estado_actual = WAIT;
            else if (x < (frame_width / 3)) { // WEBCAM detecta a la izqda
                if (area > 5500000 || (y != 0 && y < (frame_height / 3)))motores_actual = BACK_L; //cerca
                else if (area != 0 && area < 3500000)motores_actual = FWD_L; //lejos
                else motores_actual = LEFT;
            } else if (x > ((frame_width / 3)*2)) { // WEBCAM detecta a la dcha
                if (area > 5500000 || (y != 0 && y < (frame_height / 3)))motores_actual = BACK_R; //cerca
                else if (area != 0 && area < 3500000)motores_actual = FWD_R; //lejos
                else motores_actual = RIGHT;
            } else { // WEBCAM detecta centrado
                if (area > 5500000 || (y != 0 && y < (frame_height / 3)))motores_actual = BACK; //cerca
                else if (area != 0 && area < 3500000)motores_actual = FWD; //lejos
                else motores_actual = STOP;
            }
            break;
        case RET:
            motores_actual = BACK;
            sensores.sub_angle = sensores.sum_angle;
            if (sensores.sum_distance<-30) estado_actual = GIR;
            break;
        case GIR:
            motores_actual = LEFT; // Añadir sentido segun webcam
            if (sensores.br == 1 || sensores.bl == 1) {
                estado_actual = RET;
                sensores.sum_distance = 0;
            } else if (sensores.sum_angle - sensores.sub_angle > 25)estado_actual = GIR2;
            break;
        case GIR2:
            motores_actual = LEFT; // Añadir sentido segun webcam
            if (sensores.br == 1 || sensores.bl == 1) {
                estado_actual = RET;
                sensores.sum_distance = 0;
            } else if ((sensores.lightbumper & 0b00011110) == 0)estado_actual = AV; //4 lightbumps centrales
            break;
        case AV:
            motores_actual = FWD;
            if (sensores.br == 1 || sensores.bl == 1) {
                estado_actual = RET;
                sensores.sum_distance = 0;
            } else if ((sensores.lightbumper & 0b00011110) != 0)estado_actual = GIR2;
            else if ((sensores.lightbumper & 0b00100000) == 0) { // lightbump lateral izqdo
                estado_actual = AVPLUS;
                sensores.sum_distance = 0;
            }
            break;
        case AVPLUS:
            motores_actual = FWD;
            if (sensores.br == 1 || sensores.bl == 1) {
                estado_actual = RET;
                sensores.sum_distance = 0;
            } else if ((sensores.lightbumper & 0b00011110) != 0)estado_actual = GIR2;
            else if ((sensores.lightbumper & 0b00100000) != 0)estado_actual = AV;
            else if (sensores.sum_distance > 300) estado_actual = GIRPLUS;
            break;
        case GIRPLUS:
            motores_actual = RIGHT;
            if (sensores.br == 1 || sensores.bl == 1) {
                estado_actual = RET;
                sensores.sum_distance = 0;
            } else if ((sensores.lightbumper & 0b00011110) != 0)estado_actual = GIR2;
            else if ((sensores.lightbumper & 0b00100000) != 0)estado_actual = AV;
            else if ((sensores.sum_angle < 25)&&(sensores.sum_angle>-25))estado_actual = WAIT; // orientacion +- 25 grados de la que tenia al chocar
            break;
    } //end switch-case

} //end void logicaEstados(...))

/**
 * TODO: documentar
 */
void ControlRobot::moverActuadores() {
    if (motores_anterior != motores_actual) {
        switch (motores_actual) {
            case STOP:
                robot->driveDirect(VELCERO, VELCERO);
                break;
            case FWD:
                robot->driveDirect(VELAV, VELAV);
                break;
            case BACK:
                robot->driveDirect(-VELAV, -VELAV);
                break;
            case LEFT:
                robot->driveDirect(VELGIR, -VELGIR);
                break;
            case RIGHT:
                robot->driveDirect(-VELGIR, VELGIR);
                break;
            case FWD_L:
                robot->driveDirect(VELAV + VELGIR, VELAV - VELGIR);
                break;
            case FWD_R:
                robot->driveDirect(VELAV - VELGIR, VELAV + VELGIR);
                break;
            case BACK_L:
                robot->driveDirect(-VELAV + VELGIR, -VELAV - VELGIR);
                break;
            case BACK_R:
                robot->driveDirect(-VELAV - VELGIR, -VELAV + VELGIR);
                break;
        }
        motores_anterior = motores_actual;
    }
}

/**
 * TODO: documentar
 */
bool ControlRobot::condicionSalida() {
    if (sensores.buttons == 0b10000000)return true;
    return false;
}

/**
 * TODO: documentar
 */
void ControlRobot::drive(int der, int izq) {
    robot->driveDirect(der, izq);
}

/**
 * TODO: documentar
 */
void ControlRobot::computeCamaraApproach() {
    if (cam.x < (cam.frame_width / 3)) { // WEBCAM detecta a la izqda
        if (cam.area > 5500000 || (cam.y != 0 && cam.y < (cam.frame_height / 3)))motores_actual = BACK_L; //cerca
        else if (cam.area != 0 && cam.area < 3500000)motores_actual = FWD_L; //lejos
        else motores_actual = LEFT;
    } else if (cam.x > ((cam.frame_width / 3)*2)) { // WEBCAM detecta a la dcha
        if (cam.area > 5500000 || (cam.y != 0 && cam.y < (cam.frame_height / 3)))motores_actual = BACK_R; //cerca
        else if (cam.area != 0 && cam.area < 3500000)motores_actual = FWD_R; //lejos
        else motores_actual = RIGHT;
    } else { // WEBCAM detecta centrado
        if (cam.area > 5500000 || (cam.y != 0 && cam.y < (cam.frame_height / 3)))motores_actual = BACK; //cerca
        else if (cam.area != 0 && cam.area < 3500000)motores_actual = FWD; //lejos
        else motores_actual = STOP;
    }
}

/**
 * TODO: documentar
 */
void ControlRobot::computeCamaraWithObstacle() {
    if (cam.x < (cam.frame_width / 3)) { // WEBCAM detecta a la izqda
        if (cam.area > 5500000 || (cam.y != 0 && cam.y < (cam.frame_height / 3)))motores_actual = BACK_L; //cerca
        else if (cam.area != 0 && cam.area < 3500000)motores_actual = LEFT; //lejos
        else motores_actual = LEFT;
    } else if (cam.x > ((cam.frame_width / 3)*2)) { // WEBCAM detecta a la dcha
        if (cam.area > 5500000 || (cam.y != 0 && cam.y < (cam.frame_height / 3)))motores_actual = BACK_R; //cerca
        else if (cam.area != 0 && cam.area < 3500000)motores_actual = RIGHT; //lejos
        else motores_actual = RIGHT;
    } else { // WEBCAM detecta centrado
        if (cam.area > 5500000 || (cam.y != 0 && cam.y < (cam.frame_height / 3)))motores_actual = BACK; //cerca
        else if (cam.area != 0 && cam.area < 3500000)motores_actual = WAIT; //lejos
        else motores_actual = STOP;
    }
}

/**
 * TODO: documentar
 */
void ControlRobot::gotoDock() {
    robot->coverAndDock();
}



void ControlRobot::setMotores_actual(char state){
    
    motores_actual = state;

}