/**
 * File:   Lidar.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#include <limits.h>

#include "Lidar.h"
#include "YdLidarX4/YdLidarX4.h"

using namespace std;

/**
 * Lidar Class constructor
 */
Lidar::Lidar() {

    // NO ES NECESARIO CREARLO SI YA SE HA INSTANCIADO EN EL HEADER
    std::cout << "Creando objeto lidar..." << std::endl;
    myX4Lidar = YdLidarX4::YdLidarX4Controller();

    myX4Lidar.ConnectToServer();

    std::cout << "Objeto lidar creado, conectando..." << std::endl;
    if (!myX4Lidar.Connect()) {
        myX4Lidar.Reset();
    }

    std::cout << "Conexion completa" << std::endl;

    // Calcular sectores
    anglestep = 360 / NUM_SECTORS;

    // Alinear primer sector centrado en la parte frontal
    int firstsector = anglestep / 2;

    // Preparar array de indices de angulos y guardar el primero
    sectorstartangles = (int*) malloc(NUM_SECTORS * sizeof (int)); // [0:NSECTORS-1]
    sectorendangles = (int*) malloc(NUM_SECTORS * sizeof (int)); // [0:NSECTORS-1]
    sectorstartangles[0] = firstsector;
    sectorendangles[1] = firstsector;

    // Calcular indices y almacenarlos
    int i = 1;
    for (i = 1; i < NUM_SECTORS; i++) {
        sectorstartangles[i] = sectorstartangles[i - 1] + anglestep;
        sectorendangles[(i + 1) % NUM_SECTORS] = sectorstartangles[i];
    }

    sectorendangles[0] = sectorstartangles[NUM_SECTORS - 1];

    // Reservar espacio para ultima lectura

    lastSample = (int*) malloc(360 * sizeof (int));

    // Poner el lidar en modo espera

    lidarstate = LidarStates::Idle;

    // Poner condicion de salida para el thread

    runLidarThread = true;

}

/*
 * Lidar Class destructor
 */
Lidar::~Lidar() {
}

/**
 * TODO: Documentar metodo
 */
void Lidar::LidarThread() {

    while (runLidarThread) {

        // Si el lidar da un error reiniciarlo

        int health;

        while ((health = myX4Lidar.GetHealthStatus()) == YdLidarX4::LidarResponses.HEALTH_BAD) {
            std::cout << "Mirando Health" << std::endl;
            std::cout << "\tEl Health del lidar es: HEALTH_BAD" << std::endl;
            std::cout << "" << std::endl;
            myX4Lidar.Reset();
            // TODO: llamar a goIdle?? O a goToScanning?
        }
        sleep(0);

        switch (lidarstate) {

            case LidarStates::Idle:
            {
                // Esperar - No hacer nada
                sleep(10);
                break;
            }

            case LidarStates::GoToIdle:
            {
                // Detener modo escaneo y parar motor

                myX4Lidar.StopScanning();
                lidarstate = LidarStates::Idle;

                break;
            }
            case LidarStates::Scanning:
            {
                // Capturar 3 muestras

                int *muestra1, *muestra2, *muestra3;
                muestra1 = myX4Lidar.GetSampleData();
                sleep(10);
                muestra2 = myX4Lidar.GetSampleData();
                sleep(10);
                muestra3 = myX4Lidar.GetSampleData();

                // TODO: Procear: Ajustar muestras para centrarlas

                // Procesar: Convertit 0 en infinito
                int i;
                for (i = 0; i < 360; i++) {

                    if (muestra1[i] == 0) muestra1[i] = INT_MAX;
                    if (muestra2[i] == 0) muestra2[i] = INT_MAX;
                    if (muestra3[i] == 0) muestra3[i] = INT_MAX;

                }

                // Hacer medias de los valores ignorando los infinitos

                for (i = 0; i < 360; i++) {

                    if (muestra1[i] != INT_MAX && muestra2[i] != INT_MAX && muestra3[i] != INT_MAX) {
                        lastSample[i] = muestra1[i] != INT_MAX + muestra2[i] != INT_MAX + muestra3[i] != INT_MAX;
                        lastSample[i] /= 3;
                    } else lastSample[i] = INT_MAX;

                }

                // Liberar muestras para evitar memory leak
                free(muestra1);
                free(muestra2);
                free(muestra3);

                // Comprobar si la delantera esta libre

                int numObstacles = 0;

                for (i = 315; i < 405; i++) {

                    if (lastSample[i % 360] <= OBSTACLEMINDISTANCE && lastSample[(i + 1) % 360] <= OBSTACLEMINDISTANCE) {
                        numObstacles++;
                    }
                }

                if (numObstacles > 0) isFrontLibre = false;
                else isFrontLibre = true;

                // Comprobar si la trasera esta libre

                numObstacles = 0;

                for (i = 135; i < 225; i++) {

                    if (lastSample[i % 360] <= OBSTACLEMINDISTANCE && lastSample[(i + 1) % 360] <= OBSTACLEMINDISTANCE) {
                        numObstacles++;
                    }
                }

                if (numObstacles > 0) {
                    isBackLibre = false;
                } else {
                    isBackLibre = true;
                }

                isObstacle = (!isBackLibre || !isFrontLibre);

                break;
            }
            case LidarStates::GoToScanning:
            {
                myX4Lidar.StartScanning();
                lidarstate = LidarStates::Scanning;

                break;
            }
        } // switch (lidarstate)

        // Esperar un tiempo: 0 sacar y meter de ejecucion
        sleep(0);

    }
}

/**
 * Calcular a donde ir con el lidar
 */
void Lidar::computeLidarTripPersonOutOfView() {
    // Calcular por secores la distancia mas larga
    int * mediassectores = (int*) malloc(NUM_SECTORS * sizeof (int));

    // Para todos los sectores entre sus angulos...
    // Mirar todos los angulos en la ultima lectura y calcular distancias medias
    int i;
    for (i = 0; i < NUM_SECTORS; i++) {
        int j;
        for (j = sectorstartangles[i]; j < sectorendangles[i]; j++) {
            if (lastSample[j])mediassectores += lastSample[j];
            mediassectores[i] /= (sectorendangles[i] - sectorstartangles[i]);
        }
    }

    // Calcular computedangle y computeddistance
    int sectorindex = indexofSmallestElement(mediassectores, NUM_SECTORS);
    computedAngle = sectorstartangles[sectorindex] + anglestep / 2;
    computedDistance = lastSample[computedAngle] - OBSTACLEMINDISTANCE;
};

void Lidar::setLidarIdle() {

    this->lidarstate = LidarStates::Idle;
}

void Lidar::setLidarScanning() {

    this->lidarstate = LidarStates::GoToScanning;
}

int Lidar::indexofSmallestElement(int * arr, int size) {
    int index = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[index])
            index = i;
    }

    return index;
}

void Lidar::exitLidarThread() {

    runLidarThread = false;
}