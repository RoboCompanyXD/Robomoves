/**
 * File:   Lidar.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#include <limits.h>

#include "Lidar.h"
#include "YdLidarX4/YdLidarX4.h"

#define NSECTORS 8      // Numero de sectores en los que se dividirán los 360 grados
#define OBSTACLEMINDISTANCE 50

using namespace std;

/**
 * Lidar Class constructor
 */
Lidar::Lidar() {
    this->myLidar = YdLidarX4::YdLidarX4Controller();
    this->myLidar.Connect();

    // Calcular sectores
    int anglestep = 360 / NSECTORS;

    // Alinear primer sector centrado en la parte frontal
    int firstsector = anglestep / 2;

    // Preparar array de indices de angulos y guardar el primero
    sectorstartangles = (int*) malloc(NSECTORS * sizeof (int)); // [0:NSECTORS-1]
    sectorendangles = (int*) malloc(NSECTORS * sizeof (int)); // [0:NSECTORS-1]
    sectorstartangles[0] = firstsector;
    sectorendangles[1] = firstsector;

    // Calcular indices y almacenarlos
    int i = 1;
    for (i = 1; i < NSECTORS; i++) {
        sectorstartangles[i] = sectorstartangles[i - 1] + anglestep;
        sectorendangles[(i + 1) % NSECTORS] = sectorstartangles[i];
    }

    sectorendangles[0] = sectorstartangles[NSECTORS - 1];

    // Reservar espacio para ultima lectura

    lastSample = (int*) malloc(360 * sizeof (int));

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

    // TODO: introducir condición de salida
    while (1) {

        // Si el lidar da un error reiniciarlo

        int health;

        while ((health = this->myLidar.GetHealthStatus()) == YdLidarX4::LidarResponses.HEALTH_BAD) {
            std::cout << "Mirando Health" << std::endl;
            std::cout << "\tEl Health del lidar es: HEALTH_BAD" << std::endl;
            std::cout << "" << std::endl;
            this->myLidar.Reset();
        }
        sleep(0);

        switch (lidarstate) {

            case Idle:

                // Esperar - No hacer nada
                sleep(10);

                break;

            case GoToIdle:

                // Detener modo escaneo y parar motor

                myLidar->StopScanning();
                lidarstate = Idle;

                break;

            case Scanning:

                // Capturar 3 muestras

                int *muestra1, *muestra2, *muestra3;
                muestra1 = myLidar->GetSampleData();
                sleep(10);
                muestra2 = myLidar->GetSampleData();
                sleep(10);
                muestra3 = myLidar->GetSampleData();

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
                    }
                    else lastSample[i] = INT_MAX;

                }

                // Liberar muestras para evitar memory leak
                free(muestra1);
                free(muestra2);
                free(muestra3);
                
                // Comprobar si la delantera esta libre

                int nobstacles = 0;

                for (i = 315; i < 405; i++) {

                    if (lastSample[i % 360] <= OBSTACLEMINDISTANCE && lastSample[(i + 1) % 360] <= OBSTACLEMINDISTANCE) {
                        nobstacles++;
                    }
                }

                if (nobstacles > 0) isFrontLibre = false;
                else isFrontLibre = true;
                
                // Comprobar si la trasera esta libre

                int nobstacles = 0;

                for (i = 135; i < 225; i++) {

                    if (lastSample[i % 360] <= OBSTACLEMINDISTANCE && lastSample[(i + 1) % 360] <= OBSTACLEMINDISTANCE) {
                        nobstacles++;
                    }
                }

                if (nobstacles > 0) isBackLibre = false;
                else isBackLibre = true;

                break;
                
                isObstable = (!isBackLibre || !isFrontLibre);

            case GoToScanning:
                
                myLidar->StartScanning();
                lidarstate = Scanning;

                break;

        }

        // Esperar un tiempo: 0 sacar y meter de ejecucion
        sleep(0);

    }
}

/**
 * Calcular a donde ir con el lidar
 */
void Lidar::computeLidarTripPersonOutOfView() {
    
    // Calcular por secores la distancia mas larga
    
    int * mediassectores = (int*) malloc(NSECTORS * sizeof (int));
    
    // Para todos los sectores entre sus angulos...
    
    int i;
    for(i=0;i<NSECTORS;i++){
        
        // Mirar todos los angulos en la ultima lectura y calcular distancias medias
        
        int j;
        for(j=sectorstartangles[i];j<sectorendangles[i];j++){
            
            if(lastSample[j])mediassectores+= lastSample[j];
            mediassectores/=(sectorendangles[i]-sectorstartangles[i]);
            
        }
        
    }
    
    
    // TODO
};

void Lidar::setLidarIdle(){

    this->lidarstate = Idle;
}

void Lidar::setLidarScanning(){

    this->lidarstate = Scanning;
}