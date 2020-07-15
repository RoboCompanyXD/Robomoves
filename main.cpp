/**
 * File:   main.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#include <cstdlib>

#include "Lidar/Lidar.h"
#include "Lidar/YdLidarX4/YdLidarX4.h"
#include "OCVCam/OCVCam.h"
#include "ControlRobot/ControlRobot.h"

#include <iostream>
#include <thread> 
#include <stdio.h>
#include <python3.7/Python.h> 


int main(int argc, char** argv) {
	std::cout << "YdLidarx4 Creando" << std::endl;
	YdLidarX4::YdLidarX4Controller lidar = YdLidarX4::YdLidarX4Controller();
    lidar.ConnectToServer();
    
	std::cout << "YdLidarx4 Creado" << std::endl;
	std::cout << "" << std::endl;

	sleep(1);

	std::cout << "Conectando a lidar" << std::endl;

	if (!lidar.Connect())
		lidar.Reset();
	std::cout << "" << std::endl;

	sleep(0);

    int health = lidar.GetHealthStatus();
    
    while((health = lidar.GetHealthStatus())==YdLidarX4::LidarResponses.HEALTH_BAD){
    
	std::cout << "Mirando Health" << std::endl;
	std::cout << "\tEl Health del lidar es: " << ((health == YdLidarX4::LidarResponses.HEALTH_OK) ? ("HEALTH_OK") : ("HEALTH_BAD")) << std::endl;
	std::cout << "" << std::endl;
    }
	sleep(0);

	std::cout << "Obteniendo deviceinfo" << std::endl;
	struct YdLidarX4::DeviceInformation * deviceinfo  = lidar.GetDeviceInfo();
	
	if(deviceinfo != 0){
		std::cout << "\tModel Number: " << deviceinfo->modelnumber << std::endl;
		std::cout << "\tFirmware Version: " << deviceinfo->firmware_version << std::endl;
		std::cout << "\tHardware Version: " << deviceinfo->hardware_version << std::endl;
		std::cout << "\tSerial Number: " << deviceinfo->serial_number << std::endl;
	}
	
	std::cout << "" << std::endl;

	sleep(0);

	std::cout << "Iniciando escaneo" << std::endl;
	lidar.StartScanning();
	std::cout << "" << std::endl;

	sleep(0);

	std::cout << "Solicitando muestra" << std::endl;
	int *muestra1, *muestra2, *muestra3, *muestra4, *muestra5;

	muestra1 = lidar.GetSampleData();
	muestra2 = lidar.GetSampleData();
	muestra3 = lidar.GetSampleData();
	muestra4 = lidar.GetSampleData();
	muestra5 = lidar.GetSampleData();

	if (muestra1 != 0 && muestra1 != 0 && muestra2 != 0 && muestra3 != 0 && 
		muestra4 != 0 && muestra5 != 0) {
		
		int i;
		for (i = 0; i < 360; i++) {
			std::cout << "data[" << i << "] =\t" << muestra1[i] << ",\t"
					  << muestra2[i] << ",\t" << muestra3[i] << ",\t" 
					  << muestra4[i] << ",\t" << muestra5[i] << std::endl;
		}
	}
	std::cout << "" << std::endl;

	free(muestra1);
	free(muestra2);
	free(muestra3);
	free(muestra4);
	free(muestra5);

	//    sleep(1);
	//
	//    std::cout << "Reiniciando lidar" << std::endl;
	//    lidar.Disconnect();
	//    std::cout << "" << std::endl;

	sleep(1);

	std::cout << "Desconectando del lidar" << std::endl;
	lidar.Disconnect();
	std::cout << "" << std::endl;
}
