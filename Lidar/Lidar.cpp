/**
 * File:   Lidar.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#include "Lidar.h"
#include "YdLidarX4/YdLidarX4.h"

using namespace std;

/*
 * Class destructor
 */
Lidar::~Lidar() {
}

/**
 * Class constructor
 */
Lidar::Lidar() {

    this->lidar = YdLidarX4();

    this->lidar.Connect();
    
    


}

/**
 * TODO: Documentar metodo
 */
void Lidar::LidarThread() {
    while (1) {
        struct dev deviceinfo = *lidar.GetDeviceInfo();
        std::cout << "\tModel Number: " << deviceinfo.modelnumber << std::endl;
        std::cout << "\tFirmware Version: " << deviceinfo.firmware_version << std::endl;
        std::cout << "\tHardware Version: " << deviceinfo.hardware_version << std::endl;
        std::cout << "\tSerial Number: " << deviceinfo.serial_number << std::endl;
    }
}