/**
 * File:   Lidar.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#include "Lidar.h"
#include "YdLidarX4/YdLidarX4.h"

using namespace std;

/**
 * Lidar Class constructor
 */
Lidar::Lidar() {
    this->myLidar = YdLidarX4::YdLidarX4Controller();
    this->myLidar.Connect();
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

    YdLidarX4::DeviceInformation deviceinfo;
    // TODO: introducir condición de salida
    while (1) {
        // TODO: ¿¿¿qué sentido tiene estar imprimendo continuamente todo esto???

        deviceinfo = *myLidar.GetDeviceInfo();
        std::cout << "\tModel Number: " << deviceinfo.modelnumber << std::endl;
        std::cout << "\tFirmware Version: " << deviceinfo.firmware_version << std::endl;
        std::cout << "\tHardware Version: " << deviceinfo.hardware_version << std::endl;
        std::cout << "\tSerial Number: " << deviceinfo.serial_number << std::endl;
    }
}

    /**
     * Calcular a donde ir con el lidar
     */
    void Lidar::computeLidarTripPersonOutOfView() {
        // TODO
    };